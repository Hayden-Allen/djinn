#include "pch.h"
#include "mesh_instance_batch.h"
#include "core/constants.h"
#include "mesh_instance.h"
#include "asset/mesh.h"
#include "asset/animated_mesh.h"
#include "animated_mesh_instance.h"

namespace djinn
{
	mesh_instance_batch::mesh_instance_batch(wptr<mesh> const& mesh, wptr<shaders> const& shaders) :
		m_mesh(mesh),
		m_shaders(shaders)
	{
		u32 total_floats = shaders->get_base_offset_bytes() / sizeof(f32);
		for (auto const& field : shaders->get_instance_fields())
			total_floats += glsl_type_size_bytes(field.type) / sizeof(f32);
		m_floats_per_instance = total_floats;
		m_instances_per_ubo = c::shader::ubo_size_bytes / (m_floats_per_instance * sizeof(f32));
		add_block();

		for (u32 i = 0; i < c::shader::num_batch_ubos; i++)
			m_shaders->uniform_block_binding(c::uniform::instance_block_name[i], i);
	}
	mesh_instance_batch::mesh_instance_batch(mesh_instance_batch&& other) noexcept :
		m_mesh(other.m_mesh),
		m_shaders(other.m_shaders),
		m_instances(std::move(other.m_instances)),
		m_ubos(std::move(other.m_ubos)),
		m_openings(std::move(other.m_openings)),
		m_instance_indices(std::move(other.m_instance_indices)),
		m_max_instance_index(std::move(other.m_max_instance_index)),
		m_next(other.m_next),
		m_valid(other.m_valid),
		m_instances_per_ubo(other.m_instances_per_ubo),
		m_floats_per_instance(other.m_floats_per_instance)
	{}



	bool mesh_instance_batch::empty() const
	{
		return m_valid == 0;
	}
	u64 mesh_instance_batch::insert(sptr<mesh_instance> instance)
	{
		u64 index = 0;
		if (!m_openings.empty())
		{
			index = m_openings.back();
			m_openings.pop_back();
			m_instances[index] = instance;
		}
		// no openings, add id to the end and put its transform at the end
		else
		{
			index = m_next;
			m_next++;
			m_instances.push_back(instance);
		}
		set_transform_index(index);
		instance->bind(wptr(this), index);
		m_valid++;
		return index;
	}
	void mesh_instance_batch::remove(u64 const index)
	{
		ASSERT(index < m_instances.size());
		ASSERT(m_instances[index] != 0);
		// this slot is now not bound to a mesh_instance
		m_instances[index].unbind();
		m_openings.push_back(index);
		// there is one less transform being written to the UBOs
		m_valid--;
		// check if we can delete a UBO now
		if (m_valid % m_instances_per_ubo == m_instances_per_ubo - 1)
			m_ubos.pop_back();

		// slot in m_instances whose transform is at the end of the UBOs
		u64 const end_transform_index = m_max_instance_index.back();
		// bind slot who currently writes to the end to write to the now-empty slot
		m_instance_indices[end_transform_index] = m_instance_indices[index];
		// bind removed slot to write to nothing
		m_instance_indices[index] = MAX_VALUE_T(u64);
		// slot who wrote to the end no longer does, remove it from the max stack
		m_max_instance_index.pop_back();
	}
	void mesh_instance_batch::update(u64 const index, mesh_instance_field const& field)
	{
		ASSERT(index < m_instance_indices.size());
		// absolute index across all blocks
		u64 const total_index = m_instance_indices[index];
		// index of block `total_index` falls within
		u64 const block_index = total_index / m_instances_per_ubo;
		ASSERT(block_index < m_ubos.size());
		// index within above block
		u64 const transform_index = total_index % m_instances_per_ubo;
		// byte offset within block
		u64 offset_bytes = field.offset_bytes + (m_floats_per_instance * sizeof(f32) * transform_index);
		m_ubos[block_index].update(field.data.data(), (u32)field.data.size(), (u32)offset_bytes);
	}

	static void transpose(m3db_t* bone)
	{
		for (u32 r = 0; r < 4; r++)
			for (u32 c = 0; c <= r; c++)
				std::swap(bone->mat4[r * 4 + c], bone->mat4[c * 4 + r]);
	}
	static void print(m3db_t* bone)
	{
		f32 const* const m = bone->mat4;
		printf("%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n\n",
			m[0], m[4], m[8], m[12],
			m[1], m[5], m[9], m[13],
			m[2], m[6], m[10], m[14],
			m[3], m[7], m[11], m[15]);
	}

	void mesh_instance_batch::add_block()
	{
		ASSERT(m_ubos.size() <= c::shader::num_batch_ubos);
		m_ubos.emplace_back(m_instances_per_ubo * m_floats_per_instance);
	}
	void mesh_instance_batch::set_transform_index(u64 const index)
	{
		// set or create value in index list
		ASSERT(index <= m_instance_indices.size());
		if (index == m_instance_indices.size())
			m_instance_indices.push_back(m_valid);
		else
			m_instance_indices[index] = m_valid;
		// update max stack
		if (m_max_instance_index.empty() || index > m_max_instance_index.back())
			m_max_instance_index.push_back(index);
		// create new ubo if needed
		if (index / m_instances_per_ubo >= m_ubos.size())
			add_block();
	}
	void mesh_instance_batch::update_transform(u64 const index, tmat<space::OBJECT, space::WORLD> const& transform)
	{
		ASSERT(index < m_instance_indices.size());
		// absolute index across all blocks
		u64 const total_index = m_instance_indices[index];
		// index of block `total_index` falls within
		u64 const block_index = total_index / m_instances_per_ubo;
		ASSERT(block_index < m_ubos.size());
		// index within above block
		u64 const transform_index = total_index % m_instances_per_ubo;
		// byte offset within block
		u64 offset_bytes = (m_floats_per_instance * sizeof(f32) * transform_index);

		m_ubos[block_index].update(transform.e, s_floats_per_mat4, (u32)offset_bytes);

		// upload normal matrix for static meshes
		if (!m_mesh->is_animated())
		{
			tmat<space::OBJECT, space::WORLD> const& normal = transform.invert_copy().transpose_copy();
			// mat3 == mat3x4 with layout(std140), so pad columns with 0
			f32 const normal3[s_floats_per_mat3] = {
				normal.i[0],
				normal.i[1],
				normal.i[2],
				0,
				normal.j[0],
				normal.j[1],
				normal.j[2],
				0,
				normal.k[0],
				normal.k[1],
				normal.k[2],
				0,
			};
			offset_bytes += s_floats_per_mat4 * sizeof(f32);
			m_ubos[block_index].update(normal3, s_floats_per_mat3, (u32)offset_bytes);
		}
		// upload bone matrices for animated meshes
		else
		{
			ASSERT(m_mesh->is_animated());
			animated_mesh const* const am = m_mesh.get<animated_mesh>();
			sptr<animated_mesh_instance> ami = m_instances[index];
			m3db_t* const bones = ami->get_pose();
			for (u32 i = 0; i < am->get_num_bones(); i++)
			{
				offset_bytes += s_floats_per_mat4 * sizeof(f32);
				if (bones)
				{
					transpose(&bones[i]);
					// print(&bones[i]);
					f32 mat[16] = {
						0, 1, 0, 0,
						1, 0, 0, 0,
						0, 0, 1, 0,
						0, 0, 0, 1
					};
					m_ubos[block_index].update(bones[i].mat4, s_floats_per_mat4, (u32)offset_bytes);
				}
				else
					m_ubos[block_index].update(tmat<space::OBJECT, space::WORLD>().e, s_floats_per_mat4, (u32)offset_bytes);
			}
			M3D_FREE((void*)bones);
		}
	}
} // namespace djinn

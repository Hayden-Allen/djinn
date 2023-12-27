#include "pch.h"
#include "mesh_instance_batch.h"
#include "core/constants.h"
#include "mesh_instance.h"

namespace djinn
{
	mesh_instance_batch::mesh_instance_batch(wptr<mesh> const& mesh) :
		m_mesh(mesh)
	{
		s_num_ubos = mgl::get_param<u32>(GL_MAX_VERTEX_UNIFORM_BLOCKS);
		s_transforms_per_ubo = mgl::get_param<u32>(GL_MAX_UNIFORM_BLOCK_SIZE) / s_mat_size;
		add_block();
	}



	u64 mesh_instance_batch::get_count() const
	{
		return m_instances.size();
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
		instance->bind(index);
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
		// TODO check if we can delete a UBO now

		// slot in m_instances whose transform is at the end of the UBOs
		u64 const end_transform_index = m_max_transform_index.back();
		// bind slot who currently writes to the end to write to the now-empty slot
		m_transform_indices[end_transform_index] = m_transform_indices[index];
		// bind removed slot to write to nothing
		m_transform_indices[index] = MAX_VALUE_T(u64);
		// slot who wrote to the end no longer does, remove it from the max stack
		m_max_transform_index.pop_back();
	}
	void mesh_instance_batch::update(u64 const index, tmat<space::OBJECT, space::WORLD> const& transform)
	{
		ASSERT(index < m_transform_indices.size());
		u64 const total_index = m_transform_indices[index];
		u64 const block_index = total_index / s_transforms_per_ubo;
		u64 const transform_index = total_index % s_transforms_per_ubo;
		ASSERT(block_index < m_transforms.size());
		m_transforms[block_index].update(transform.e, s_floats_per_mat, (u32)transform_index * s_mat_size);
	}
	void mesh_instance_batch::draw(sptr<mgl::context> const& ctx, static_render_object const& ro, sptr<shaders> const& shaders)
	{
		for (u64 i = 0; i < m_instances.size(); i++)
			update(i, m_instances[i]->get_world_transform());
		// bind first block to 0, so all blocks will be bound in [0, n)
		shaders->uniform_block_binding(c::uniform::instanced_transforms_block, 0);
		for (u32 i = 0; i < (u32)m_transforms.size(); i++)
		{
			m_transforms[i].bind(i);
		}
		ctx->draw_instanced(ro, *shaders.get(), m_valid);
	}



	void mesh_instance_batch::add_block()
	{
		m_transforms.emplace_back(s_transforms_per_ubo * s_floats_per_mat);
	}
	void mesh_instance_batch::set_transform_index(u64 const index)
	{
		// set or create value in index list
		ASSERT(index <= m_transform_indices.size());
		if (index == m_transform_indices.size())
			m_transform_indices.push_back(m_valid);
		else
			m_transform_indices[index] = m_valid;
		// update max stack
		if (m_max_transform_index.empty() || index > m_max_transform_index.back())
			m_max_transform_index.push_back(index);
		// create new ubo if needed
		if (index / s_transforms_per_ubo > m_transforms.size())
			add_block();
	}
} // namespace djinn
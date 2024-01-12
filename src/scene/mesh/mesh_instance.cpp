#include "pch.h"
#include "mesh_instance.h"
#include "asset/mesh/mesh.h"
#include "mesh_instance_batch.h"
#include "asset/shaders.h"

namespace djinn
{
	mesh_instance::mesh_instance(id_t const id, sptr<mesh> const& mesh, wptr<shaders> const& batch_shaders) :
		visibility_scene_object(id),
		m_mesh(mesh),
		m_batch_index(MAX_VALUE_T(u64)),
		m_batch_shaders(batch_shaders),
		m_batch(nullptr)
	{
		auto const& fields = batch_shaders->get_instance_fields();
		for (shaders::instance_field const& f : fields)
		{
			u32 const num_floats = glsl_type_size_bytes(f.type) / sizeof(f32) * f.arr_count;
			std::vector<f32> data(num_floats);
			m_fields.push_back({ data, f.type, f.arr_count, f.offset_bytes });
			m_field_index.insert({ f.name, m_fields.size() - 1 });
		}
	}



	void mesh_instance::set_uniform(std::string const& name, std::vector<f32> const& data, u32 const index)
	{
		auto it = m_field_index.find(name);
		ASSERT(it != m_field_index.end());
		mesh_instance_field& field = m_fields.at(it->second);
		ASSERT(index < (u32)field.arr_count);
		ASSERT(data.size() * sizeof(f32) == glsl_type_size_bytes(field.type));
		std::copy(data.begin(), data.end(), field.data.begin() + index);
		m_batch->update(m_batch_index, field);
	}
	bool mesh_instance::is_animated() const
	{
		return false;
	}



	void mesh_instance::remove_from_batch()
	{
		m_mesh->remove_instance(m_batch_shaders, m_batch_index);
	}



	void mesh_instance::bind(wptr<mesh_instance_batch> batch, u64 const index)
	{
		m_batch = batch;
		m_batch_index = index;
	}
} // namespace djinn

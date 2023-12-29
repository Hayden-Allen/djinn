#include "pch.h"
#include "mesh_instance.h"
#include "mesh.h"
#include "mesh_instance_batch.h"
#include "asset/shaders.h"

namespace djinn
{
	mesh_instance::mesh_instance(id_t const id, wptr<mesh> const& mesh, sptr<shaders> const& batch_shaders) :
		scene_object(id),
		m_mesh(mesh),
		m_batch_index(MAX_VALUE_T(u64)),
		m_batch_shaders(batch_shaders)
	{
		auto const& fields = batch_shaders->get_instance_fields();
		for (shaders::instance_field const& f : fields)
		{
			std::vector<f32> data(mgl::get_shader_type_size(f.type) * f.arr_count);
			m_fields.insert({ f.name, { data, f.type, f.arr_count } });
		}
	}



	void mesh_instance::remove_from_batch()
	{
		m_mesh->remove_instance(m_batch_shaders, m_batch_index);
	}



	void mesh_instance::bind(u64 const index)
	{
		m_batch_index = index;
	}
} // namespace djinn

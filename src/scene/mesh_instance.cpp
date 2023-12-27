#include "pch.h"
#include "mesh_instance.h"
#include "mesh.h"

namespace djinn
{
	mesh_instance::mesh_instance(id_t const id) :
		scene_object(id),
		m_batch_index(MAX_VALUE_T(u64))
	{}



	void mesh_instance::bind(u64 const index)
	{
		m_batch_index = index;
	}
} // namespace djinn

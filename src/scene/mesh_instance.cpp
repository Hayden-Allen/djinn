#include "pch.h"
#include "mesh_instance.h"

namespace djinn
{
	mesh_instance::mesh_instance(id_t const id, u64 const index) :
		scene_object(id),
		m_batch_index(index)
	{}
} // namespace djinn

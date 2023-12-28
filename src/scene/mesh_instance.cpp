#include "pch.h"
#include "mesh_instance.h"
#include "mesh.h"
#include "mesh_instance_batch.h"

namespace djinn
{
	mesh_instance::mesh_instance(id_t const id) :
		scene_object(id),
		m_batch_index(MAX_VALUE_T(u64)),
		m_batch(nullptr)
	{}



	void mesh_instance::remove_from_batch()
	{
		ASSERT(m_batch);
		m_batch->remove(m_batch_index);
	}



	void mesh_instance::bind(mesh_instance_batch* const batch, u64 const index)
	{
		m_batch_index = index;
		m_batch = batch;
	}
} // namespace djinn

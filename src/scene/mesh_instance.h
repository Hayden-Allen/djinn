#pragma once
#include "pch.h"
#include "scene_object.h"

namespace djinn
{
	class mesh_instance final : public scene_object
	{
		friend class mesh_instance_batch;
	public:
		DCM(mesh_instance);
	private:
		u64 m_batch_index;
	private:
		mesh_instance(id_t const id, u64 const index);
	};
} // namespace djinn

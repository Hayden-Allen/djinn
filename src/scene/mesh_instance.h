#pragma once
#include "pch.h"
#include "scene_object.h"

namespace djinn
{
	class mesh;
	class mesh_instance_batch;

	class mesh_instance final : public scene_object
	{
		friend class mesh_instance_batch;
	public:
		mesh_instance(id_t const id);
		DCM(mesh_instance);
	public:
		void remove_from_batch();
	private:
		u64 m_batch_index;
		mesh_instance_batch* m_batch;
	private:
		void bind(mesh_instance_batch* const batch, u64 const index);
	};
} // namespace djinn

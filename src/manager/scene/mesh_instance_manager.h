#pragma once
#include "pch.h"
#include "manager/manager.h"
#include "scene/mesh/mesh_instance.h"

namespace djinn
{
	class mesh;
	class shaders;

	class mesh_instance_manager final : public ref_counted_manager<mesh_instance>
	{
	public:
		mesh_instance_manager();
		DCM(mesh_instance_manager);
	public:
		id_t create(sptr<mesh> mesh, sptr<shaders> shaders);
		void destroy(id_t const id) override;
	};
} // namespace djinn
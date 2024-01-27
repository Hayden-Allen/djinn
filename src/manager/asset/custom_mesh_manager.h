#pragma once
#include "pch.h"
#include "manager/manager.h"
#include "asset/mesh/custom_mesh.h"

namespace djinn
{
	class custom_mesh_manager final : public ref_counted_manager<custom_mesh>
	{
	public:
		custom_mesh_manager();
		DCM(custom_mesh_manager);
	public:
		id_t create(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<custom_mesh_texture> const& textures);
	};
} // namespace djinn

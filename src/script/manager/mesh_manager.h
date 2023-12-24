#pragma once
#include "pch.h"
#include "manager.h"

namespace djinn
{
	class mesh_manager final : public ref_counted_manager<static_render_object>
	{
	public:
		mesh_manager();
		DCM(mesh_manager);
	public:
		id_t create(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count);
		id_t load(std::string const& fp) override;
		void destroy(id_t const id) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void update(id_t const id, std::vector<f32> const& vertices, std::vector<u32> const& indices);
	};
} // namespace djinn

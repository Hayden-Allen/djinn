#include "pch.h"
#include "mesh_manager.h"
#include "core/constants.h"

namespace djinn
{
	mesh_manager::mesh_manager() :
		ref_counted_manager(c::base_dir::mesh)
	{}



	id_t mesh_manager::create(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count)
	{
		static_render_object* const ro = new static_render_object(nullptr, vertex_count, layout, nullptr, index_count);
		return insert(ro);
	}
	id_t mesh_manager::load(std::string const& fp)
	{
		ASSERT(false)
		return create(0, {}, 0);
	}
	void mesh_manager::destroy(id_t const id)
	{
		try_erase(id);
	}
	void mesh_manager::reload(std::string const& fp)
	{
		ASSERT(false)
	}
	void mesh_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		ASSERT(false)
	}
	void mesh_manager::update(id_t const id, std::vector<f32> const& vertices, std::vector<u32> const& indices)
	{
		sptr<static_render_object> ro = get(id);
		ro->get_vertex_array().get_vertices().update(vertices.data(), (u32)vertices.size(), 0);
		ro->get_indices().update(indices.data(), (u32)indices.size(), 0);
	}
} // namespace djinn

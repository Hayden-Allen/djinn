#include "pch.h"
#include "mesh_manager.h"
#include "core/constants.h"

namespace djinn
{
	mesh_manager::mesh_manager() :
		ref_counted_file_manager(c::base_dir::mesh)
	{}



	id_t mesh_manager::create(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<wptr<texture>> const& textures)
	{
		return insert(new generated_mesh(vertex_count, layout, index_count, textures));
	}
	id_t mesh_manager::load(std::string const& fp)
	{
		ASSERT(false)
		return 0;
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
		get(id)->update(vertices, indices);
	}
} // namespace djinn

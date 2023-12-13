#include "pch.h"
#include "mesh_manager.h"
#include "constants.h"

namespace djinn
{
	mesh_manager::mesh_manager() :
		manager(c::mesh_manager::base_dir)
	{}
	mesh_manager::~mesh_manager()
	{}



	id_t mesh_manager::create(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count)
	{
		static_render_object* const ro = new static_render_object(nullptr, vertex_count, layout, nullptr, index_count);
		return insert(ro);
	}
	id_t mesh_manager::load(std::string const& fp)
	{
		ASSERT(false);
		return create(0, {}, 0);
	}
	void mesh_manager::remove(id_t const id)
	{
		erase(id);
	}
	void mesh_manager::reload(std::string const& fp)
	{
		ASSERT(false);
	}
	void mesh_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		ASSERT(false);
	}
} // namespace djinn

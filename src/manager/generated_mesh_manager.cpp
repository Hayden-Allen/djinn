#include "pch.h"
#include "generated_mesh_manager.h"
#include "core/constants.h"

namespace djinn
{
	generated_mesh_manager::generated_mesh_manager() :
		ref_counted_manager()
	{}



	id_t generated_mesh_manager::create(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<wptr<texture>> const& textures)
	{
		return insert(new generated_mesh(vertex_count, layout, index_count, textures));
	}
	void generated_mesh_manager::destroy(id_t const id)
	{
		try_erase(id);
	}
	void generated_mesh_manager::update(id_t const id, std::vector<f32> const& vertices, std::vector<u32> const& indices)
	{
		get(id)->update(vertices, indices);
	}
} // namespace djinn

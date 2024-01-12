#include "pch.h"
#include "custom_mesh_manager.h"
#include "core/constants.h"

namespace djinn
{
	custom_mesh_manager::custom_mesh_manager() :
		ref_counted_manager()
	{}



	id_t custom_mesh_manager::create(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<wptr<texture>> const& textures)
	{
		return insert(new custom_mesh(vertex_count, layout, index_count, textures));
	}
	void custom_mesh_manager::update(id_t const id, std::vector<f32> const& vertices, std::vector<u32> const& indices)
	{
		get(id)->update(vertices, indices);
	}
} // namespace djinn

#include "pch.h"
#include "custom_mesh.h"
#include "asset/shaders.h"
#include "scene/mesh/mesh_instance_batch.h"

namespace djinn
{
	custom_mesh::custom_mesh(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<mesh_texture> const& textures) :
		mesh(textures),
		m_ro(vertex_count, layout, index_count)
	{}



	void custom_mesh::update(std::vector<f32> const& vertices)
	{
		m_ro.update(vertices.data(), (u32)vertices.size());
	}
	void custom_mesh::update(std::vector<u32> const& indices)
	{
		m_ro.update(indices.data(), (u32)indices.size());
	}
	void custom_mesh::update(std::vector<f32> const& vertices, std::vector<u32> const& indices)
	{
		m_ro.update(vertices.data(), (u32)vertices.size(), indices.data(), (u32)indices.size());
	}
	void custom_mesh::draw(sptr<mgl::context> const& ctx)
	{
		mesh::draw(ctx);
		for (auto& pair : m_batches)
			for (auto& batch : pair.second)
				batch->draw(ctx, m_ro);
	}
} // namespace djinn

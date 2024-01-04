#include "pch.h"
#include "custom_mesh.h"
#include "asset/shaders.h"

namespace djinn
{
	custom_mesh::custom_mesh(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<wptr<texture>> const& textures) :
		m_ro(vertex_count, layout, index_count),
		m_textures(textures)
	{}



	void custom_mesh::update(std::vector<f32> const& vertices, std::vector<u32> const& indices)
	{
		m_ro.update(vertices.data(), (u32)vertices.size(), indices.data(), (u32)indices.size());
	}
	void custom_mesh::draw(sptr<mgl::context> const& ctx)
	{
		for (u64 i = 0; i < m_textures.size(); i++)
			m_textures[i]->bind((u32)i);
		for (auto& pair : m_batches)
			pair.second.draw(ctx, m_ro);
	}
} // namespace djinn

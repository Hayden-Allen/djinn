#include "pch.h"
#include "custom_mesh.h"
#include "asset/shaders.h"
#include "scene/mesh/mesh_instance_batch.h"

namespace djinn
{
	custom_mesh::custom_mesh(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<custom_mesh_texture> const& textures) :
		m_ro(vertex_count, layout, index_count),
		m_textures(textures)
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
		for (u64 i = 0; i < m_textures.size(); i++)
		{
			if (m_textures[i].is_raw)
				m_textures[i].raw->bind((u32)i);
			else
				m_textures[i].arr->bind((u32)i);
		}
		for (auto& pair : m_batches)
			for (auto& batch : pair.second)
				batch->draw(ctx, m_ro);
	}
} // namespace djinn

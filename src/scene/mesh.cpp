#include "pch.h"
#include "mesh.h"
#include "asset/shaders.h"

namespace djinn
{
	mesh::mesh(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<wptr<texture>> const& textures) :
		m_ro(vertex_count, layout, index_count),
		m_textures(textures)
	{}



	void mesh::update(std::vector<f32> const& vertices, std::vector<u32> const& indices)
	{
		m_ro.update(vertices.data(), (u32)vertices.size(), indices.data(), (u32)indices.size());
	}
	void mesh::draw(sptr<mgl::context> const& ctx)
	{
		for (u64 i = 0; i < m_textures.size(); i++)
			m_textures[i]->bind((u32)i);
		for (auto& pair : m_batches)
			pair.second.draw(ctx, m_ro);
	}



	void mesh::insert_instance(wptr<shaders> const& x, sptr<mesh_instance> instance)
	{
		auto it = m_batches.find(x);
		if (it == m_batches.end())
		{
			m_batches.try_emplace(x, wptr(this), x);
		}
		m_batches.at(x).insert(instance);
	}
	void mesh::remove_instance(wptr<shaders> const& shaders, u64 const index)
	{
		m_batches.at(shaders).remove(index);
		if (m_batches.at(shaders).empty())
			m_batches.erase(shaders);
	}
} // namespace djinn

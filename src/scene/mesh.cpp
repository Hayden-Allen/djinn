#include "pch.h"
#include "mesh.h"

namespace djinn
{
	mesh::mesh(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<sptr<texture>> const& textures, sptr<shaders> const& shaders) :
		m_ro(vertex_count, layout, index_count),
		m_textures(textures),
		m_batch(wptr(this)),
		m_shaders(shaders)
	{}



	void mesh::update(std::vector<f32> const& vertices, std::vector<u32> const& indices)
	{
		m_ro.update(vertices.data(), (u32)vertices.size(), indices.data(), (u32)indices.size());
	}
	void mesh::draw(sptr<mgl::context> const& ctx)
	{
		m_shaders->bind();
		for (u64 i = 0; i < m_textures.size(); i++)
			m_textures[i]->bind((u32)i);
		m_batch.draw(ctx, m_ro, m_shaders);
	}
	void mesh::set_shaders(sptr<shaders> const& shaders)
	{
		m_shaders = shaders;
	}



	void mesh::insert_instance(sptr<mesh_instance> instance)
	{
		m_batch.insert(instance);
	}
} // namespace djinn

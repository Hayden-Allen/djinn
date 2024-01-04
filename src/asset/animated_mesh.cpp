#include "pch.h"
#include "animated_mesh.h"

namespace djinn
{
	animated_mesh::animated_mesh(m3d_t* const raw)
	{
		init(raw);
	}


	void animated_mesh::draw(sptr<mgl::context> const& ctx)
	{
		for (auto& pair : m_batches)
			pair.second.draw(ctx, m_ro);
	}
	bool animated_mesh::is_animated() const
	{
		return true;
	}
	void animated_mesh::init(m3d_t* const raw)
	{
		// ASSERT(raw->numbone > 0);
		m_raw = raw;

		std::vector<vertex> vbo;
		std::unordered_map<vertex, u32> vert2idx;
		std::vector<u32> ibo;

		for (u32 i = 0; i < raw->numface; i++)
		{
			m3df_t const& cur = raw->face[i];
			for (u32 j = 0; j < 3; j++)
			{
				m3dv_t pos, norm;
				m3dti_t tex;
				m3ds_t skin;
				if (cur.vertex[j] < raw->numvertex)
				{
					pos = raw->vertex[cur.vertex[j]];
					ASSERT(pos.skinid < raw->numskin);
					skin = raw->skin[pos.skinid];
				}
				if (cur.normal[j] < raw->numvertex)
					norm = raw->vertex[cur.normal[j]];
				if (cur.texcoord[j] < raw->numtmap)
					tex = raw->tmap[cur.texcoord[j]];
				vertex v(pos, norm, tex, skin);
				auto const& it = vert2idx.find(v);
				if (it != vert2idx.end())
				{
					ibo.push_back(it->second);
				}
				else
				{
					u32 const idx = (u32)vbo.size();
					vbo.push_back(v);
					vert2idx.insert({ v, idx });
					ibo.push_back(idx);
				}
			}
		}

		m_ro.init(vbo.data(), vbo.size(), get_layout(), ibo.data(), ibo.size());
	}
	m3db_t* animated_mesh::get_pose() const
	{
		static u32 frame = 0;
		return m3d_pose(m_raw, 0, frame++);
	}
} // namespace djinn

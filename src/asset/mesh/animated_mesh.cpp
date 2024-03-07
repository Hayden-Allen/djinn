#include "pch.h"
#include "animated_mesh.h"
#include "core/m3d.h"

namespace djinn
{
	animated_mesh::animated_mesh(m3d_t* const raw, std::vector<mesh_texture> const& textures) :
		loaded_mesh(textures),
		m_raw(raw)
	{
		init(raw);
		for (u32 i = 0; i < raw->numbone; i++)
		{
			m_name2idx.insert({ raw->bone[i].name, i });
		}
	}



	bool animated_mesh::is_animated() const
	{
		return true;
	}
	void animated_mesh::init(m3d_t* const raw)
	{
		ASSERT(raw->numbone <= c::shader::num_bones);
		m_raw = raw;

		std::vector<animated_mesh_vertex> vbo;
		std::unordered_map<animated_mesh_vertex, u32> vert2idx;
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
				animated_mesh_vertex v(pos, norm, tex, skin);
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

		action const bind = { "bind", 0, -1 };
		m_actions.insert({ "bind", bind });
		for (u32 i = 0; i < raw->numaction; i++)
		{
			m3da_t const& cur = raw->action[i];
			action const a = { cur.name, cur.durationmsec, (s32)i };
			m_actions.insert({ cur.name, a });
		}
	}
	u32 animated_mesh::get_num_bones() const
	{
		return m_raw->numbone;
	}


	m3db_t* animated_mesh::get_pose(s32 const id, f32 const sec) const
	{
		m3db_t* const anim = m3d_pose(m_raw, id, (u32)(sec * 1000));
		for (u32 i = 0; i < m_raw->numbone; i++)
		{
			f32 out[16] = { 0 };
			m3d::mat4_mul_mat4(out, anim[i].mat4, m_raw->bone[i].mat4);
			// m3d mats are row-major
			m3d::mat4_transpose(out);
			memcpy(anim[i].mat4, out, 16 * sizeof(f32));
		}
		return anim;
	}
	u32 animated_mesh::get_bone_index(std::string const& name) const
	{
		ASSERT(m_name2idx.contains(name));
		return m_name2idx.at(name);
	}
} // namespace djinn

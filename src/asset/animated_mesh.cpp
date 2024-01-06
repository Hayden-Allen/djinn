#include "pch.h"
#include "animated_mesh.h"

namespace djinn
{
	static void _m3d_mul(M3D_FLOAT* r, M3D_FLOAT* a, M3D_FLOAT* b)
	{
		r[0] = b[0] * a[0] + b[4] * a[1] + b[8] * a[2] + b[12] * a[3];
		r[1] = b[1] * a[0] + b[5] * a[1] + b[9] * a[2] + b[13] * a[3];
		r[2] = b[2] * a[0] + b[6] * a[1] + b[10] * a[2] + b[14] * a[3];
		r[3] = b[3] * a[0] + b[7] * a[1] + b[11] * a[2] + b[15] * a[3];
		r[4] = b[0] * a[4] + b[4] * a[5] + b[8] * a[6] + b[12] * a[7];
		r[5] = b[1] * a[4] + b[5] * a[5] + b[9] * a[6] + b[13] * a[7];
		r[6] = b[2] * a[4] + b[6] * a[5] + b[10] * a[6] + b[14] * a[7];
		r[7] = b[3] * a[4] + b[7] * a[5] + b[11] * a[6] + b[15] * a[7];
		r[8] = b[0] * a[8] + b[4] * a[9] + b[8] * a[10] + b[12] * a[11];
		r[9] = b[1] * a[8] + b[5] * a[9] + b[9] * a[10] + b[13] * a[11];
		r[10] = b[2] * a[8] + b[6] * a[9] + b[10] * a[10] + b[14] * a[11];
		r[11] = b[3] * a[8] + b[7] * a[9] + b[11] * a[10] + b[15] * a[11];
		r[12] = b[0] * a[12] + b[4] * a[13] + b[8] * a[14] + b[12] * a[15];
		r[13] = b[1] * a[12] + b[5] * a[13] + b[9] * a[14] + b[13] * a[15];
		r[14] = b[2] * a[12] + b[6] * a[13] + b[10] * a[14] + b[14] * a[15];
		r[15] = b[3] * a[12] + b[7] * a[13] + b[11] * a[14] + b[15] * a[15];
	}



	animated_mesh::animated_mesh(m3d_t* const raw) :
		m_raw(raw)
	{
		init(raw);
	}



	bool animated_mesh::is_animated() const
	{
		return true;
	}
	void animated_mesh::init(m3d_t* const raw)
	{
		// ASSERT(raw->numbone > 0);
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



	m3db_t* animated_mesh::get_pose(s32 const id, f32 const sec)
	{
		// m3db_t* const anim = m3d_pose(m_raw, m_current_action->id, (u32)delta);
		m3db_t* const anim = m3d_pose(m_raw, id, (u32)(sec * 1000));
		for (u32 i = 0; i < m_raw->numbone; i++)
		{
			f32 out[16] = { 0 };
			_m3d_mul(out, anim[i].mat4, m_raw->bone[i].mat4);
			memcpy(anim[i].mat4, out, sizeof(f32) * 16);
		}
		return anim;
	}
} // namespace djinn

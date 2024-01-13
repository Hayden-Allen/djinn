#pragma once
#include "pch.h"
#include "loaded_mesh.h"
#include "core/util.h"

namespace djinn
{
	class animated_mesh_instance;
	struct animated_mesh_vertex
	{
		f32 pos[3], norm[3], tex[2], bone_weight[c::shader::num_vertex_bones];
		u8 bone_idx[c::shader::num_vertex_bones];

		animated_mesh_vertex(m3dv_t const& _pos, m3dv_t const& _norm, m3dti_t const& _tex, m3ds_t const& _skin)
		{
			pos[0] = _pos.x;
			pos[1] = _pos.y;
			pos[2] = _pos.z;
			norm[0] = _norm.x;
			norm[1] = _norm.y;
			norm[2] = _norm.z;
			tex[0] = _tex.u;
			tex[1] = _tex.v;
			for (u32 i = 0; i < c::shader::num_vertex_bones; i++)
			{
				bone_weight[i] = _skin.weight[i];
				bone_idx[i] = _skin.boneid[i];
			}
			// printf("%u %f\n", bone_idx[0], bone_weight[0]);
		}

		bool operator==(animated_mesh_vertex const& o) const
		{
			for (u32 i = 0; i < c::shader::num_vertex_bones; i++)
			{
				if (bone_weight[i] != o.bone_weight[i]) return false;
				if (bone_idx[i] != o.bone_idx[i]) return false;
			}
			return pos[0] == o.pos[0] && pos[1] == o.pos[1] && pos[2] == o.pos[2] && norm[0] == o.norm[0] && norm[1] == o.norm[1] && norm[2] == o.norm[2] && tex[0] == o.tex[0] && tex[1] == o.tex[1];
		}
	};



	class animated_mesh final : public loaded_mesh<animated_mesh_vertex>
	{
		friend animated_mesh_instance;
	public:
		struct action
		{
			std::string name;
			u32 msec;
			s32 id;
		};
	public:
		animated_mesh(m3d_t* const raw);
		DCM(animated_mesh);
	public:
		void init(m3d_t* const raw) override;
		bool is_animated() const override;
		u32 get_num_bones() const;
	protected:
		std::vector<std::pair<GLenum, std::vector<u32>>> get_layout() const override
		{
			return {
				{ GL_FLOAT, { 3, 3, 2, c::shader::num_vertex_bones } },
				{ GL_UNSIGNED_BYTE, { c::shader::num_vertex_bones } }
			};
		}
		std::vector<tmat<space::OBJECT, space::WORLD>> get_pose(s32 const id, f32 const sec);
		u32 get_bone_index(std::string const& name) const;
	private:
		m3d_t* m_raw;
		std::unordered_map<std::string, action> m_actions;
		// std::vector<tmat<space::OBJECT, space::WORLD>> m_last_bones;
		std::unordered_map<std::string, u32> m_name2idx;
	};
} // namespace djinn

template<>
struct std::hash<djinn::animated_mesh_vertex>
{
	u64 operator()(djinn::animated_mesh_vertex const& v) const
	{
		u64 hash = 0;
		hash = djinn::u::hash_combine(hash, v.pos[0]);
		hash = djinn::u::hash_combine(hash, v.pos[1]);
		hash = djinn::u::hash_combine(hash, v.pos[2]);
		hash = djinn::u::hash_combine(hash, v.norm[0]);
		hash = djinn::u::hash_combine(hash, v.norm[1]);
		hash = djinn::u::hash_combine(hash, v.norm[2]);
		hash = djinn::u::hash_combine(hash, v.tex[0]);
		hash = djinn::u::hash_combine(hash, v.tex[1]);
		for (u32 i = 0; i < djinn::c::shader::num_vertex_bones; i++)
		{
			hash = djinn::u::hash_combine(hash, v.bone_weight[i]);
			hash = djinn::u::hash_combine(hash, v.bone_idx[i]);
		}
		return hash;
	}
};

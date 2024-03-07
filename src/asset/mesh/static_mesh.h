#pragma once
#include "pch.h"
#include "loaded_mesh.h"
#include "core/util.h"

namespace djinn
{
	struct static_mesh_vertex
	{
		f32 pos[3], norm[3], tex[2];

		static_mesh_vertex(m3dv_t const& _pos, m3dv_t const& _norm, m3dti_t const& _tex)
		{
			pos[0] = _pos.x;
			pos[1] = _pos.y;
			pos[2] = _pos.z;
			norm[0] = _norm.x;
			norm[1] = _norm.y;
			norm[2] = _norm.z;
			tex[0] = _tex.u;
			tex[1] = _tex.v;
		}

		bool operator==(static_mesh_vertex const& o) const
		{
			return pos[0] == o.pos[0] && pos[1] == o.pos[1] && pos[2] == o.pos[2] && norm[0] == o.norm[0] && norm[1] == o.norm[1] && norm[2] == o.norm[2] && tex[0] == o.tex[0] && tex[1] == o.tex[1];
		}
	};



	class static_mesh final : public loaded_mesh<static_mesh_vertex>
	{
	public:
	public:
		static_mesh(m3d_t* const raw, std::vector<mesh_texture> const& textures);
		DCM(static_mesh);
	public:
		void init(m3d_t* const raw) override;
	protected:
		std::vector<std::pair<GLenum, std::vector<u32>>> get_layout() const override
		{
			return {
				{ GL_FLOAT, { 3, 3, 2 } }
			};
		}
	};
} // namespace djinn

template<>
struct std::hash<djinn::static_mesh_vertex>
{
	u64 operator()(djinn::static_mesh_vertex const& v) const
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
		return hash;
	}
};

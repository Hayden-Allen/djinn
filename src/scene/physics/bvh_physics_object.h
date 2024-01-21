#pragma once
#include "pch.h"
#include "physics_object.h"

namespace djinn
{
	class phorm;

	struct power_vertex
	{
		f32 pos[3];
		power_vertex(f32 const* const raw)
		{
			for (u32 i = 0; i < 3; i++)
				pos[i] = raw[i];
		}
		bool operator==(power_vertex const& other) const
		{
			return pos[0] == other.pos[0] && pos[1] == other.pos[1] && pos[2] == other.pos[2];
		}
	};

	class bvh_physics_object final : public physics_object
	{
	public:
		bvh_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, sptr<phorm> const& phorm);
		DCM(bvh_physics_object);
		~bvh_physics_object();
	private:
		optr<btTriangleIndexVertexArray> m_iva;
	};
} // namespace djinn

template<>
struct std::hash<djinn::power_vertex>
{
	u64 operator()(djinn::power_vertex const& v) const
	{
		u64 hash = 0;
		for (u32 i = 0; i < 3; i++)
			hash = djinn::u::hash_combine(hash, v.pos[i]);
		return hash;
	}
};

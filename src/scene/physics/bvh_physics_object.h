#pragma once
#include "pch.h"
#include "physics_object.h"

namespace djinn
{
	class phorm;

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

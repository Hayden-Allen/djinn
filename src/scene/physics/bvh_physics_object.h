#pragma once
#include "pch.h"
#include "physics_object.h"

namespace djinn
{
	class bvh_physics_object final : public physics_object
	{
	public:
		bvh_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, static_retained_render_object* const ro);
		DCM(bvh_physics_object);
		~bvh_physics_object();
	private:
		optr<btTriangleIndexVertexArray> m_iva;
	};
} // namespace djinn

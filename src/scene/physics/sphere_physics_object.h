#pragma once
#include "pch.h"
#include "primitive_physics_object.h"

namespace djinn
{
	class sphere_physics_object final : public primitive_physics_object
	{
	public:
		sphere_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, f32 const radius, btVector3 const& origin, f32 const mass);
		DCM(sphere_physics_object);
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "primitive_physics_object.h"

namespace djinn
{
	class cylinder_physics_object final : public primitive_physics_object
	{
	public:
		cylinder_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, btVector3 const& dims, btVector3 const& origin, f32 const mass);
		DCM(cylinder_physics_object);
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "physics_object.h"

namespace djinn
{
	class primitive_physics_object : public physics_object
	{
	public:
		DCM(primitive_physics_object);
	protected:
		primitive_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, btCollisionShape* const shape, btVector3 const& origin, f32 const mass);
	};
} // namespace djinn

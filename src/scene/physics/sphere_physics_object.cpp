#include "pch.h"
#include "sphere_physics_object.h"

namespace djinn
{
	sphere_physics_object::sphere_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, f32 const radius, btVector3 const& origin, f32 const mass) :
		primitive_physics_object(id, world, new btSphereShape(radius), origin, mass)
	{}
} // namespace djinn

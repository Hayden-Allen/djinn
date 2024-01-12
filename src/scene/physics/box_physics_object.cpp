#include "pch.h"
#include "box_physics_object.h"

namespace djinn
{
	box_physics_object::box_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, btVector3 const& dims, btVector3 const& origin, f32 const mass) :
		primitive_physics_object(id, world, new btBoxShape(dims), origin, mass)
	{}
} // namespace djinn

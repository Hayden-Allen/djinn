#include "pch.h"
#include "cylinder_physics_object.h"

namespace djinn
{
	cylinder_physics_object::cylinder_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, btVector3 const& dims, btVector3 const& origin, f32 const mass) :
		primitive_physics_object(id, world, new btCylinderShape(dims), origin, mass)
	{}
} // namespace djinn

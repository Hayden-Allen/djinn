#include "pch.h"
#include "capsule_physics_object.h"

namespace djinn
{
	capsule_physics_object::capsule_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, axis const axis, f32 const radius, f32 const height, btVector3 const& origin, f32 const mass) :
		primitive_physics_object(id, world, create_shape(axis, radius, height), origin, mass)
	{}



	btCollisionShape* capsule_physics_object::create_shape(axis const axis, f32 const radius, f32 const height)
	{
		switch (axis)
		{
		case axis::X: return new btCapsuleShapeX(radius, height); break;
		case axis::Y: return new btCapsuleShape(radius, height); break;
		case axis::Z: return new btCapsuleShapeZ(radius, height); break;
		}
		ASSERT(false);
		return nullptr;
	}
} // namespace djinn

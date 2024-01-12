#pragma once
#include "pch.h"
#include "primitive_physics_object.h"

namespace djinn
{
	class box_physics_object final : public primitive_physics_object
	{
	public:
		box_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, btVector3 const& dims, btVector3 const& origin, f32 const mass);
		DCM(box_physics_object);
	};
} // namespace djinn

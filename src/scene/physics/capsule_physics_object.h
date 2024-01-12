#pragma once
#include "pch.h"
#include "primitive_physics_object.h"

namespace djinn
{
	class capsule_physics_object final : public primitive_physics_object
	{
	public:
		enum class axis
		{
			NONE = -1,
			X,
			Y,
			Z,
			COUNT
		};
	public:
		capsule_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, axis const axis, f32 const radius, f32 const height, btVector3 const& origin, f32 const mass);
		DCM(capsule_physics_object);
	private:
		static btCollisionShape* create_shape(axis const axis, f32 const radius, f32 const height);
	};
} // namespace djinn

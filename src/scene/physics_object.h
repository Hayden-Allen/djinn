#pragma once
#include "pch.h"
#include "scene_object.h"

namespace djinn
{
	class physics_object final : public scene_object
	{
		friend class physics_object_manager;
	public:
		physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, btVector3 const& dims, btVector3 const& origin, f32 const mass);
		DCM(physics_object);
		~physics_object();
	public:
		void update_transform() override;
		void copy_physics_transform();
	private:
		sptr<btDiscreteDynamicsWorld> m_world;
		optr<btRigidBody> m_rb;
		optr<btMotionState> m_motion;
		optr<btCollisionShape> m_shape;
	};
} // namespace djinn

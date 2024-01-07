#pragma once
#include "pch.h"
#include "scene_object_base.h"

namespace djinn
{
	class physics_object final : public scene_object_base
	{
		friend class physics_object_manager;
	public:
		physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, btVector3 const& dims, btVector3 const& origin, f32 const mass);
		DCM(physics_object);
		~physics_object();
	public:
		void update_transform() override;
		tmat<space::OBJECT, space::PARENT> get_transform() const override;
		tmat<space::OBJECT, space::WORLD> get_world_transform() const override;
		void set_linear_velocity(f32 const x, f32 const y, f32 const z);
		void set_angular_velocity(f32 const x, f32 const y, f32 const z);
	private:
		sptr<btDiscreteDynamicsWorld> m_world;
		optr<btRigidBody> m_rb;
		optr<btMotionState> m_motion;
		optr<btCollisionShape> m_shape;
	private:
		void copy_physics_transform();
	};
} // namespace djinn

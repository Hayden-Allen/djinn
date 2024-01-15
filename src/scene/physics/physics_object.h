#pragma once
#include "pch.h"
#include "scene/scene_object_base.h"
#include "core/util.h"

namespace djinn
{
	class physics_object : public scene_object_base
	{
		friend class physics_object_manager;
	public:
		DCM(physics_object);
		virtual ~physics_object();
	public:
		void set_friction(f32 const f);
		void set_velocity(f32 const x, f32 const y, f32 const z);
		void set_velocity_x(f32 const x);
		void set_velocity_y(f32 const y);
		void set_velocity_z(f32 const z);
		void set_velocity_local(f32 const x, f32 const y, f32 const z);
		void set_velocity_local_x(f32 const x);
		void set_velocity_local_y(f32 const y);
		void set_velocity_local_z(f32 const z);
		void set_angular_velocity(f32 const x, f32 const y, f32 const z);
		void set_collision_enabled(bool const enabled);
		void set_angular_factor(f32 const x, f32 const y, f32 const z);
	protected:
		sptr<btDiscreteDynamicsWorld> m_world;
		optr<btRigidBody> m_rb;
		optr<btMotionState> m_motion;
		optr<btCollisionShape> m_shape;
	protected:
		physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world);
	protected:
		void copy_physics_transform();
	};
} // namespace djinn

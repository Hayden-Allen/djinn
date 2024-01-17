#pragma once
#include "pch.h"
#include "scene/scene_object.h"
#include "core/util.h"

namespace djinn
{
	class entity;

	class physics_object : public scene_object
	{
		friend class physics_object_manager;
	public:
		DCM(physics_object);
		virtual ~physics_object();
	public:
		void set_friction(f32 const f);
		vec<space::WORLD> get_velocity_world() const;
		void set_velocity_world(f32 const x, f32 const y, f32 const z);
		void set_velocity_x_world(f32 const x);
		void set_velocity_y_world(f32 const y);
		void set_velocity_z_world(f32 const z);
		vec<space::OBJECT> get_velocity() const;
		void set_velocity(f32 const x, f32 const y, f32 const z);
		void set_velocity_x(f32 const x);
		void set_velocity_y(f32 const y);
		void set_velocity_z(f32 const z);
		void set_angular_velocity(f32 const x, f32 const y, f32 const z);
		void set_collision_enabled(bool const enabled);
		void set_angular_factor(f32 const x, f32 const y, f32 const z);
		void apply_impulse(vec<space::OBJECT> const& force);
		void set_damping(f32 const d);
		void set_angular_damping(f32 const d);
		void set_max_speed(u32 const index, f32 const max);
	protected:
		sptr<btDiscreteDynamicsWorld> m_world;
		optr<btRigidBody> m_rb;
		optr<btMotionState> m_motion;
		optr<btCollisionShape> m_shape;
		entity* m_entity;
		f32 m_max_speed[3] = { 0 };
	protected:
		physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world);
	protected:
		void clamp_velocity();
		void copy_transform_to_physics();
		void copy_transform_from_physics();
		void check_collisions();
	};
} // namespace djinn

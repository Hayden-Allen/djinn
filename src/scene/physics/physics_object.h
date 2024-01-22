#pragma once
#include "pch.h"
#include "scene/scene_object.h"
#include "core/util.h"

namespace djinn
{
	class phorm;
	class entity;
	class physics_object;
	class sweep_test_callback final : public btCollisionWorld::ClosestConvexResultCallback
	{
	public:
		sweep_test_callback(btCollisionObject* me, btVector3 const& up, btScalar minSlopeDot) :
			btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)), m_me(me), m_up(up), m_minSlopeDot(minSlopeDot)
		{}
		DCM(sweep_test_callback);
	public:
		btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace) override;
	protected:
		btCollisionObject* m_me;
		const btVector3 m_up;
		btScalar m_minSlopeDot;
	};
	class contact_test_callback final : public btCollisionWorld::ContactResultCallback
	{
	public:
		contact_test_callback(physics_object const* const self) :
			me(self)
		{}
		DCM(contact_test_callback);
	public:
		btScalar addSingleResult(btManifoldPoint& cp, btCollisionObjectWrapper const* colObj0Wrap, int partId0, int index0, btCollisionObjectWrapper const* colObj1Wrap, int partId1, int index1) override;
	private:
		physics_object const* me;
	};

	class physics_object : public scene_object
	{
		friend class physics_object_manager;
		friend class contact_test_callback;
	public:
		DCM(physics_object);
		virtual ~physics_object();
	public:
		void bind(phorm* const p);
		void bind(entity* const e);
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
		void set_gravity(vec<space::WORLD> const& force);
		void set_kinematic(bool const is_kinematic);
		void set_ghost(bool const is_ghost);
		void collide_and_slide(vec<space::OBJECT> const& vel, f32 const dt);
	protected:
		sptr<btDiscreteDynamicsWorld> m_world;
		optr<btRigidBody> m_rb;
		optr<btMotionState> m_motion;
		optr<btCollisionShape> m_shape;
		union
		{
			phorm* p;
			entity* e;
		} m_bound;
		f32 m_max_speed[3] = { 0 };
		bool m_bound_is_entity;
	protected:
		physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world);
	protected:
		void bind_to_bullet();
		void clamp_velocity();
		void copy_transform_to_physics();
		void copy_transform_from_physics();
		void check_collisions();
	};
} // namespace djinn

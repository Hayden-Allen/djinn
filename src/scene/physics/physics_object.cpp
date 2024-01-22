#include "pch.h"
#include "physics_object.h"
#include "core/util.h"
#include "scene/tagged.h"
#include "scene/entity/entity.h"
#include "core/constants.h"

namespace djinn
{
	btScalar sweep_test_callback::addSingleResult(btCollisionWorld::LocalConvexResult& convexResult, bool normalInWorldSpace)
	{
		if (convexResult.m_hitCollisionObject == m_me)
			return btScalar(1.0);

		if (!convexResult.m_hitCollisionObject->hasContactResponse())
			return btScalar(1.0);

		btVector3 hitNormalWorld;
		if (normalInWorldSpace)
		{
			hitNormalWorld = convexResult.m_hitNormalLocal;
		}
		else
		{
			/// need to transform normal into worldspace
			hitNormalWorld = convexResult.m_hitCollisionObject->getWorldTransform().getBasis() * convexResult.m_hitNormalLocal;
		}

		btScalar dotUp = m_up.dot(hitNormalWorld);
		if (dotUp < m_minSlopeDot)
		{
			return btScalar(1.0);
		}

		return ClosestConvexResultCallback::addSingleResult(convexResult, normalInWorldSpace);
	}
	btScalar contact_test_callback::addSingleResult(btManifoldPoint& cp, btCollisionObjectWrapper const* colObj0Wrap, int partId0, int index0, btCollisionObjectWrapper const* colObj1Wrap, int partId1, int index1)
	{
		direction<space::WORLD> const& normal = u::bullet2direction<space::WORLD>(cp.m_normalWorldOnB);
		physics_object* const obj0 = (physics_object*)colObj0Wrap->getCollisionObject()->getUserPointer();
		physics_object* const obj1 = (physics_object*)colObj1Wrap->getCollisionObject()->getUserPointer();
		// only call __collide if I am an entity and the other object is bound to something
		if (obj0 && obj0 == me && obj0->m_bound_is_entity && obj1 && obj1->m_bound.e)
		{
			if (obj1->m_bound_is_entity)
				obj0->m_bound.e->call_collide(obj1->m_bound.e, normal);
			else
				obj0->m_bound.e->call_collide(obj1->m_bound.p, normal);
		}
		else if (obj1 && obj1 == me && obj1->m_bound_is_entity && obj0 && obj0->m_bound.e)
		{
			if (obj0->m_bound_is_entity)
				obj1->m_bound.e->call_collide(obj0->m_bound.e, normal);
			else
				obj1->m_bound.e->call_collide(obj0->m_bound.p, normal);
		}
		return 0.f;
	}


	physics_object::~physics_object()
	{
		m_world->removeRigidBody(m_rb.get());
		m_rb.free();
		m_motion.free();
		m_shape.free();
	}



	void physics_object::bind(phorm* const p)
	{
		m_bound.p = p;
		m_bound_is_entity = false;
	}
	void physics_object::bind(entity* const e)
	{
		m_bound.e = e;
		m_bound_is_entity = true;
	}
	void physics_object::set_friction(f32 const f)
	{
		m_rb->setFriction(f);
	}
	vec<space::WORLD> physics_object::get_velocity_world() const
	{
		return u::bullet2vec<space::WORLD>(m_rb->getLinearVelocity());
	}
	void physics_object::set_velocity_world(f32 const x, f32 const y, f32 const z)
	{
		m_rb->setLinearVelocity(btVector3(x, y, z));
	}
	void physics_object::set_velocity_x_world(f32 const x)
	{
		btVector3 v = m_rb->getLinearVelocity();
		v.setX(x);
		m_rb->setLinearVelocity(v);
	}
	void physics_object::set_velocity_y_world(f32 const y)
	{
		btVector3 v = m_rb->getLinearVelocity();
		v.setY(y);
		m_rb->setLinearVelocity(v);
	}
	void physics_object::set_velocity_z_world(f32 const z)
	{
		btVector3 v = m_rb->getLinearVelocity();
		v.setZ(z);
		m_rb->setLinearVelocity(v);
	}
	vec<space::OBJECT> physics_object::get_velocity() const
	{
		vec<space::WORLD> const w = u::bullet2vec<space::WORLD>(m_rb->getLinearVelocity());
		return get_world_transform().invert_copy() * w;
	}
	void physics_object::set_velocity(f32 const x, f32 const y, f32 const z)
	{
		btVector3 const v(x, y, z);
		btVector3 const world = m_rb->getWorldTransform().getBasis() * v;
		m_rb->setLinearVelocity(world);
	}
	void physics_object::set_velocity_x(f32 const x)
	{
		btVector3 local = m_rb->getWorldTransform().getBasis().transpose() * m_rb->getLinearVelocity();
		local.setX(x);
		set_velocity(local.x(), local.y(), local.z());
	}
	void physics_object::set_velocity_y(f32 const y)
	{
		btVector3 local = m_rb->getWorldTransform().getBasis().transpose() * m_rb->getLinearVelocity();
		local.setY(y);
		set_velocity(local.x(), local.y(), local.z());
	}
	void physics_object::set_velocity_z(f32 const z)
	{
		btVector3 local = m_rb->getWorldTransform().getBasis().transpose() * m_rb->getLinearVelocity();
		local.setZ(z);
		set_velocity(local.x(), local.y(), local.z());
	}
	void physics_object::set_angular_velocity(f32 const x, f32 const y, f32 const z)
	{
		m_rb->setAngularVelocity(btVector3(x, y, z));
	}
	void physics_object::set_collision_enabled(bool const enabled)
	{
		if (enabled)
			m_rb->setCollisionFlags(m_rb->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			m_rb->setCollisionFlags(m_rb->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
	void physics_object::set_angular_factor(f32 const x, f32 const y, f32 const z)
	{
		m_rb->setAngularFactor(btVector3(x, y, z));
	}
	void physics_object::apply_impulse(vec<space::OBJECT> const& force)
	{
		vec<space::WORLD> const& w = get_world_transform() * force;
		m_rb->applyCentralImpulse(u::vec2bullet(w));
	}
	void physics_object::set_damping(f32 const d)
	{
		m_rb->setDamping(d, m_rb->getAngularDamping());
	}
	void physics_object::set_angular_damping(f32 const d)
	{
		m_rb->setDamping(m_rb->getLinearDamping(), d);
	}
	void physics_object::set_max_speed(u32 const index, f32 const max)
	{
		ASSERT(index < 3);
		m_max_speed[index] = max;
	}
	void physics_object::set_gravity(vec<space::WORLD> const& force)
	{
		m_rb->setGravity(u::vec2bullet(force));
	}
	void physics_object::set_kinematic(bool const is_kinematic)
	{
		if (is_kinematic)
			m_rb->setCollisionFlags(m_rb->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		else
			m_rb->setCollisionFlags(m_rb->getCollisionFlags() & ~btCollisionObject::CF_KINEMATIC_OBJECT);
	}
	void physics_object::set_ghost(bool const is_ghost)
	{
		if (is_ghost)
			m_rb->setCollisionFlags(m_rb->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_STATIC_OBJECT);
		else
			m_rb->setCollisionFlags(m_rb->getCollisionFlags() & ~btCollisionObject::CF_NO_CONTACT_RESPONSE & ~btCollisionObject::CF_STATIC_OBJECT);
	}
	void physics_object::collide_and_slide(vec<space::OBJECT> const& vel, f32 const dt)
	{
		// ASSERT(m_rb->getFlags() & btCollisionObject::CF_KINEMATIC_OBJECT);
		tmat<space::OBJECT, space::WORLD> const& mat = get_world_transform();
		vec<space::WORLD> const& up = mat.get_j();
		btCollisionShape const* const raw_shape = m_rb->getCollisionShape();
		ASSERT(raw_shape->isConvex());
		btConvexShape const* const shape = (btConvexShape const*)raw_shape;

		vec<space::WORLD> vel_remaining = mat * vel;
		vec<space::WORLD> vel_total(0, 0, 0);
		for (u32 i = 0; i < 8 && vel_remaining.length2() > 0; i++)
		{
			sweep_test_callback cb(m_rb.get(), u::vec2bullet(up), .5);
			btTransform from = u::tmat2bullet(mat);
			from.setOrigin(from.getOrigin() + u::vec2bullet(vel_total * dt));
			btTransform to = from;
			to.setOrigin(from.getOrigin() + u::vec2bullet(vel_remaining * dt));
			m_world->convexSweepTest(shape, from, to, cb);
			if (!cb.hasHit())
			{
				vel_total += vel_remaining;
				break;
			}
			vec<space::WORLD> const vel_before = vel_remaining * std::max(::c::EPSILON, cb.m_closestHitFraction);
			vel_total += vel_before;

			vec<space::WORLD> const vel_after = vel_remaining - vel_before;
			vec<space::WORLD> const normal = u::bullet2vec<space::WORLD>(cb.m_hitNormalWorld);
			vel_remaining = vel_after - vel_after.dot(normal) * normal;
		}

		m_rb->setLinearVelocity(u::vec2bullet(vel_total));
	}



	physics_object::physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world) :
		scene_object(id),
		m_world(world),
		m_bound({ nullptr }),
		m_bound_is_entity(false)
	{
		for (u32 i = 0; i < 3; i++)
			m_max_speed[i] = MAX_VALUE_T(f32);
	}



	void physics_object::bind_to_bullet()
	{
		m_rb->setUserPointer((void*)this);
	}
	void physics_object::clamp_velocity()
	{
		/*vec<space::WORLD> vel = u::bullet2vec<space::WORLD>(m_rb->getLinearVelocity());
		if (vel.length() > m_max_speed)
			vel.set_length(m_max_speed);
		m_rb->setLinearVelocity(u::vec2bullet(vel));*/
		btVector3 vel = m_rb->getLinearVelocity();
		vel[0] = u::clamp(vel[0], -m_max_speed[0], m_max_speed[0]);
		vel[1] = u::clamp(vel[1], -m_max_speed[1], m_max_speed[1]);
		vel[2] = u::clamp(vel[2], -m_max_speed[2], m_max_speed[2]);
		m_rb->setLinearVelocity(vel);
	}
	void physics_object::copy_transform_to_physics()
	{
		tmat<space::OBJECT, space::WORLD> const& mat = get_world_transform();
		btTransform const& raw = u::tmat2bullet(mat);
		m_rb->getMotionState()->setWorldTransform(raw);
	}
	void physics_object::copy_transform_from_physics()
	{
		btTransform raw;
		m_rb->getMotionState()->getWorldTransform(raw);
		m_rb->setWorldTransform(raw);
		tmat<space::OBJECT, space::WORLD> const& mat = u::bullet2tmat<space::OBJECT, space::WORLD>(raw);
		m_transform = get_parent_transform().invert_copy() * mat;
		u::extract_rot(m_transform, &m_rot[0], &m_rot[1], &m_rot[2]);
	}
	void physics_object::check_collisions()
	{
		contact_test_callback cb(this);
		m_world->contactTest(m_rb.get(), cb);
	}
} // namespace djinn

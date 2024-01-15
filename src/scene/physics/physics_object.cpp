#include "pch.h"
#include "physics_object.h"
#include "core/util.h"

namespace djinn
{
	physics_object::~physics_object()
	{
		m_world->removeRigidBody(m_rb.get());
		m_rb.free();
		m_motion.free();
		m_shape.free();
	}



	void physics_object::set_friction(f32 const f)
	{
		m_rb->setFriction(f);
	}
	void physics_object::set_velocity(f32 const x, f32 const y, f32 const z)
	{
		m_rb->setLinearVelocity(btVector3(x, y, z));
	}
	void physics_object::set_velocity_x(f32 const x)
	{
		btVector3 v = m_rb->getLinearVelocity();
		v.setX(x);
		m_rb->setLinearVelocity(v);
	}
	void physics_object::set_velocity_y(f32 const y)
	{
		btVector3 v = m_rb->getLinearVelocity();
		v.setY(y);
		m_rb->setLinearVelocity(v);
	}
	void physics_object::set_velocity_z(f32 const z)
	{
		btVector3 v = m_rb->getLinearVelocity();
		v.setZ(z);
		m_rb->setLinearVelocity(v);
	}
	void physics_object::set_velocity_local(f32 const x, f32 const y, f32 const z)
	{
		btVector3 const v(x, y, z);
		btVector3 const world = m_rb->getWorldTransform().getBasis() * v;
		m_rb->setLinearVelocity(world);
	}
	void physics_object::set_velocity_local_x(f32 const x)
	{
		btVector3 local = m_rb->getWorldTransform().getBasis().transpose() * m_rb->getLinearVelocity();
		local.setX(x);
		set_velocity_local(local.x(), local.y(), local.z());
	}
	void physics_object::set_velocity_local_y(f32 const y)
	{
		btVector3 local = m_rb->getWorldTransform().getBasis().transpose() * m_rb->getLinearVelocity();
		local.setY(y);
		set_velocity_local(local.x(), local.y(), local.z());
	}
	void physics_object::set_velocity_local_z(f32 const z)
	{
		btVector3 local = m_rb->getWorldTransform().getBasis().transpose() * m_rb->getLinearVelocity();
		local.setZ(z);
		set_velocity_local(local.x(), local.y(), local.z());
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



	physics_object::physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world) :
		scene_object(id),
		m_world(world)
	{}



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
		tmat<space::OBJECT, space::WORLD> const& mat = u::bullet2tmat<space::OBJECT, space::WORLD>(raw);
		m_transform = get_parent_transform().invert_copy() * mat;
	}
} // namespace djinn

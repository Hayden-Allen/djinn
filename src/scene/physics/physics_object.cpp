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



	void physics_object::update_transform()
	{
		tmat<space::OBJECT, space::PARENT> const& trans = tmat_util::translation<space::OBJECT, space::PARENT>(m_pos[0], m_pos[1], m_pos[2]);
		tmat<space::OBJECT, space::OBJECT> const& rot = tmat_util::rotation_yxz<space::OBJECT>(m_rot[0], m_rot[1], m_rot[2]);
		tmat<space::OBJECT, space::OBJECT> const& scale = tmat_util::scale<space::OBJECT>(m_scale[0], m_scale[1], m_scale[2]);
		m_transform = trans * rot * scale;
		// TODO mat * accumulate_parent_mats().invert_copy()
		// m_rb->getMotionState()->setWorldTransform(u::tmat2bullet(mat));
	}
	tmat<space::OBJECT, space::PARENT> physics_object::get_transform() const
	{
		// tmat<space::OBJECT, space::WORLD> const& obj2world = get_world_transform();
		//// TODO return obj2world * accumulate_parent_mats().invert_copy();
		// return obj2world.cast_copy<space::OBJECT, space::PARENT>();
		return m_transform;
	}
	tmat<space::OBJECT, space::WORLD> physics_object::get_world_transform() const
	{
		/*btTransform mat;
		m_rb->getMotionState()->getWorldTransform(mat);
		return u::bullet2tmat<space::OBJECT, space::WORLD>(mat);*/
		return m_transform.cast_copy<space::OBJECT, space::WORLD>();
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
		btVector3 const world = m_rb->getWorldTransform().getBasis().transpose() * v;
		m_rb->setLinearVelocity(world);
	}
	void physics_object::set_velocity_local_x(f32 const x)
	{
		btVector3 local = m_rb->getWorldTransform().getBasis() * m_rb->getLinearVelocity();
		local.setX(x);
		set_velocity_local(local.x(), local.y(), local.z());
	}
	void physics_object::set_velocity_local_y(f32 const y)
	{
		btVector3 local = m_rb->getWorldTransform().getBasis() * m_rb->getLinearVelocity();
		local.setY(y);
		set_velocity_local(local.x(), local.y(), local.z());
	}
	void physics_object::set_velocity_local_z(f32 const z)
	{
		btVector3 local = m_rb->getWorldTransform().getBasis() * m_rb->getLinearVelocity();
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
		scene_object_base(id),
		m_world(world)
	{}



	void physics_object::copy_physics_transform()
	{
		btTransform raw;
		m_rb->getMotionState()->getWorldTransform(raw);
		// TODO multiply by accumulate_parent_mats.invert_copy();
		tmat<space::OBJECT, space::PARENT> const& mat = u::bullet2tmat<space::OBJECT, space::PARENT>(raw);
		extract_transform(mat);
		update_transform();
	}
	void physics_object::extract_physics_transform(btTransform const& world)
	{
		extract_transform(u::bullet2tmat<space::OBJECT, space::PARENT>(world));
		update_transform();
	}
} // namespace djinn

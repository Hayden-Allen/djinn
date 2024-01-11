#include "pch.h"
#include "physics_object.h"
#include "core/util.h"

namespace djinn
{
	physics_object::physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, btVector3 const& dims, btVector3 const& origin, f32 const mass) :
		scene_object_base(id),
		m_world(world)
	{
		m_shape = new btBoxShape(dims);

		btTransform mat;
		mat.setIdentity();
		mat.setOrigin(origin);
		m_motion = new btDefaultMotionState(mat);

		btVector3 inertia(0, 0, 0);
		if (mass != 0.f)
			m_shape->calculateLocalInertia(mass, inertia);

		btRigidBody::btRigidBodyConstructionInfo info(mass, m_motion.get(), m_shape.get(), inertia);
		m_rb = new btRigidBody(info);
		m_rb->setActivationState(DISABLE_DEACTIVATION);
		m_world->addRigidBody(m_rb.get());
	}
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
		tmat<space::OBJECT, space::PARENT> const& mat = trans * rot * scale;
		// TODO mat * accumulate_parent_mats().invert_copy()
		m_rb->getMotionState()->setWorldTransform(u::tmat2bullet(mat));
	}
	tmat<space::OBJECT, space::PARENT> physics_object::get_transform() const
	{
		tmat<space::OBJECT, space::WORLD> const& obj2world = get_world_transform();
		// TODO return obj2world * accumulate_parent_mats().invert_copy();
		return obj2world.cast_copy<space::OBJECT, space::PARENT>();
	}
	tmat<space::OBJECT, space::WORLD> physics_object::get_world_transform() const
	{
		btTransform mat;
		m_rb->getMotionState()->getWorldTransform(mat);
		return u::bullet2tmat<space::OBJECT, space::WORLD>(mat);
	}
	void physics_object::set_friction(f32 const f)
	{
		m_rb->setFriction(f);
	}
	void physics_object::set_linear_velocity(f32 const x, f32 const y, f32 const z)
	{
		m_rb->setLinearVelocity(btVector3(x, y, z));
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



	void physics_object::copy_physics_transform()
	{
		btTransform raw;
		m_rb->getMotionState()->getWorldTransform(raw);
		// TODO multiply by accumulate_parent_mats.invert_copy();
		tmat<space::OBJECT, space::PARENT> const& mat = u::bullet2tmat<space::OBJECT, space::PARENT>(raw);
		m_pos[0] = mat.t[0];
		m_pos[1] = mat.t[1];
		m_pos[2] = mat.t[2];
		m_scale[0] = mat.get_i().length();
		m_scale[1] = mat.get_j().length();
		m_scale[2] = mat.get_k().length();

		m_rot[0] = asin(mat.m[2][1]);
		if (abs(mat.m[2][1]) < 0.9999999)
		{
			m_rot[1] = atan2(mat.m[0][2], mat.m[2][2]);
			m_rot[2] = atan2(mat.m[0][1], mat.m[1][1]);
		}
		else
		{
			m_rot[1] = atan2(-mat.m[0][2], mat.m[0][0]);
			m_rot[2] = 0;
		}
	}
} // namespace djinn

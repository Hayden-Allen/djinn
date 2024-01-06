#include "pch.h"
#include "physics_object.h"
#include "core/util.h"

namespace djinn
{
	physics_object::physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, btVector3 const& dims, btVector3 const& origin, f32 const mass) :
		scene_object(id),
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
		scene_object::update_transform();
		tmat<space::OBJECT, space::WORLD> const& wt = get_world_transform();
		m_rb->getMotionState()->setWorldTransform(u::tmat2bullet(wt));
	}
	void physics_object::copy_physics_transform()
	{
		btTransform mat;
		m_rb->getMotionState()->getWorldTransform(mat);
		tmat<space::OBJECT, space::WORLD> const& wt = u::bullet2tmat<space::OBJECT, space::WORLD>(mat);
		set_world_transform(wt);
	}
} // namespace djinn

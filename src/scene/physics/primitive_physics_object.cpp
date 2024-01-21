#include "pch.h"
#include "primitive_physics_object.h"

namespace djinn
{
	primitive_physics_object::primitive_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, btCollisionShape* const shape, btVector3 const& origin, f32 const mass) :
		physics_object(id, world)
	{
		m_shape = shape;

		set_pos_world(point<space::WORLD>(origin.x(), origin.y(), origin.z()));

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

		bind_to_bullet();
	}
} // namespace djinn

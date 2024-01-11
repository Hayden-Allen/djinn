#include "pch.h"
#include "physics_object_manager.h"

namespace djinn
{
	physics_object_manager::physics_object_manager()
	{
		m_config = new btDefaultCollisionConfiguration();
		m_dispatcher = new btCollisionDispatcher(m_config.get());
		m_cache = new btDbvtBroadphase();
		m_solver = new btSequentialImpulseConstraintSolver();
		m_world = new btDiscreteDynamicsWorld(m_dispatcher.get(), m_cache.get(), m_solver.get(), m_config.get());
		m_world->setGravity(btVector3(0, -10, 0));
	}
	physics_object_manager::~physics_object_manager()
	{
		m_config.free();
		m_dispatcher.free();
		m_cache.free();
		m_solver.free();
		m_world.free();
	}



	id_t physics_object_manager::create(btVector3 const& dims, btVector3 const& origin, f32 const mass)
	{
		id_t const id = insert(new physics_object(s_next_id, m_world, dims, origin, mass));
		return id;
	}
	void physics_object_manager::update(f32 const dt)
	{
		for (auto& pair : m_objects)
			pair.second->update_transform();
		m_world->stepSimulation(dt, 8);
		for (auto& pair : m_objects)
			pair.second->copy_physics_transform();
	}
} // namespace djinn

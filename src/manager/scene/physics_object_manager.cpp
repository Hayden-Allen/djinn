#include "pch.h"
#include "physics_object_manager.h"
#include "scene/xport/phorm.h"
#include "scene/physics/box_physics_object.h"
#include "scene/physics/cylinder_physics_object.h"
#include "scene/physics/bvh_physics_object.h"
#include "scene/physics/capsule_physics_object.h"
#include "scene/physics/sphere_physics_object.h"

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



	id_t physics_object_manager::create_box(btVector3 const& dims, btVector3 const& origin, f32 const mass)
	{
		return insert(new box_physics_object(s_next_id, m_world, dims, origin, mass));
	}
	id_t physics_object_manager::create_cylinder(btVector3 const& dims, btVector3 const& origin, f32 const mass)
	{
		return insert(new cylinder_physics_object(s_next_id, m_world, dims, origin, mass));
	}
	id_t physics_object_manager::create_sphere(f32 const radius, btVector3 const& origin, f32 const mass)
	{
		return insert(new sphere_physics_object(s_next_id, m_world, radius, origin, mass));
	}
	id_t physics_object_manager::create_capsule_x(f32 const radius, f32 const height, btVector3 const& origin, f32 const mass)
	{
		return insert(new capsule_physics_object(s_next_id, m_world, capsule_physics_object::axis::X, radius, height, origin, mass));
	}
	id_t physics_object_manager::create_capsule_y(f32 const radius, f32 const height, btVector3 const& origin, f32 const mass)
	{
		return insert(new capsule_physics_object(s_next_id, m_world, capsule_physics_object::axis::Y, radius, height, origin, mass));
	}
	id_t physics_object_manager::create_capsule_z(f32 const radius, f32 const height, btVector3 const& origin, f32 const mass)
	{
		return insert(new capsule_physics_object(s_next_id, m_world, capsule_physics_object::axis::Z, radius, height, origin, mass));
	}
	id_t physics_object_manager::create_bvh(sptr<phorm> const& phorm)
	{
		return insert(new bvh_physics_object(s_next_id, m_world, phorm));
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

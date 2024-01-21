#include "pch.h"
#include "physics_object_manager.h"
#include "scene/xport/phorm.h"
#include "scene/physics/box_physics_object.h"
#include "scene/physics/cylinder_physics_object.h"
#include "scene/physics/bvh_physics_object.h"
#include "scene/physics/capsule_physics_object.h"
#include "scene/physics/sphere_physics_object.h"
#include "debug/physics_debug_drawer.h"
#include "core/constants.h"

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
#ifndef DJINN_DIST
		m_drawer = new physics_debug_drawer();
		m_world->setDebugDrawer(m_drawer.get());
#endif
	}
	physics_object_manager::~physics_object_manager()
	{
#ifndef DJINN_DIST
		m_drawer.free();
#endif
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
		{
			pair.second->clamp_velocity();
			pair.second->copy_transform_to_physics();
		}
		s32 const taken = m_world->stepSimulation(dt, c::physics::num_substeps, c::physics::substep_time);
		for (auto& pair : m_objects)
		{
			pair.second->copy_transform_from_physics();
			pair.second->check_collisions();
		}
	}
	std::vector<raycast_result> physics_object_manager::cast_ray(point<space::WORLD> const& from, direction<space::WORLD> const& dir, f32 const length)
	{
		point<space::WORLD> const& to = from + dir * length;
		btVector3 const& bfrom = u::point2bullet(from);
		btVector3 const& bto = u::point2bullet(to);

		btCollisionWorld::AllHitsRayResultCallback allResults(bfrom, bto);
		allResults.m_flags |= btTriangleRaycastCallback::kF_KeepUnflippedNormal;
		allResults.m_flags |= btTriangleRaycastCallback::kF_UseSubSimplexConvexCastRaytest;
		m_world->rayTest(bfrom, bto, allResults);

		std::vector<raycast_result> ret;
		ret.reserve(allResults.m_hitFractions.size());
		for (s32 i = 0; i < allResults.m_hitFractions.size(); i++)
		{
			point<space::WORLD> const& point = u::bullet2point<space::WORLD>(allResults.m_hitPointWorld[i]);
			direction<space::WORLD> const& norm = u::bullet2direction<space::WORLD>(allResults.m_hitNormalWorld[i]);
			ret.emplace_back(point, norm, allResults.m_hitFractions[i]);
		}
		std::sort(ret.begin(), ret.end());
		return ret;
	}
#ifndef DJINN_DIST
	void physics_object_manager::debug_draw(mat<space::WORLD, space::CLIP> const& vp)
	{
		m_drawer->set_camera(vp);
		m_world->debugDrawWorld();
		m_drawer->flushLines();
	}
#endif
} // namespace djinn

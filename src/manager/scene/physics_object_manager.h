#pragma once
#include "pch.h"
#include "manager/manager.h"
#include "scene/physics/physics_object.h"

namespace djinn
{
	class phorm;
	class physics_debug_drawer;

	struct raycast_result
	{
		point<space::WORLD> point;
		direction<space::WORLD> normal;
		f32 frac;

		bool operator<(raycast_result const& o) const
		{
			return frac < o.frac;
		}
	};

	class physics_object_manager final : public ref_counted_manager<physics_object>
	{
	public:
		physics_object_manager();
		DCM(physics_object_manager);
		~physics_object_manager();
	public:
		id_t create_box(btVector3 const& dims, btVector3 const& origin, f32 const mass);
		id_t create_cylinder(btVector3 const& dims, btVector3 const& origin, f32 const mass);
		id_t create_sphere(f32 const radius, btVector3 const& origin, f32 const mass);
		id_t create_capsule_x(f32 const radius, f32 const height, btVector3 const& origin, f32 const mass);
		id_t create_capsule_y(f32 const radius, f32 const height, btVector3 const& origin, f32 const mass);
		id_t create_capsule_z(f32 const radius, f32 const height, btVector3 const& origin, f32 const mass);
		id_t create_bvh(sptr<phorm> const& phorm);
		void update(f32 const dt);
		std::vector<raycast_result> cast_ray(point<space::WORLD> const& from, direction<space::WORLD> const& dir, f32 const length);
#ifndef DJINN_DIST
		void debug_draw(mat<space::WORLD, space::CLIP> const& vp);
#endif
	private:
		optr<btDefaultCollisionConfiguration> m_config;
		optr<btCollisionDispatcher> m_dispatcher;
		optr<btBroadphaseInterface> m_cache;
		optr<btSequentialImpulseConstraintSolver> m_solver;
		optr<btDiscreteDynamicsWorld> m_world;
#ifndef DJINN_DIST
		optr<physics_debug_drawer> m_drawer;
#endif
	};
} // namespace djinn

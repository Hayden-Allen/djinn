#pragma once
#include "pch.h"
#include "manager/manager.h"
#include "scene/physics/physics_object.h"

namespace djinn
{
	class phorm;

	class physics_object_manager final : public ref_counted_manager<physics_object>
	{
	public:
		physics_object_manager();
		DCM(physics_object_manager);
		~physics_object_manager();
	public:
		id_t create_box(btVector3 const& dims, btVector3 const& origin, f32 const mass);
		id_t create_sphere(f32 const radius, btVector3 const& origin, f32 const mass);
		id_t create_capsule_x(f32 const radius, f32 const height, btVector3 const& origin, f32 const mass);
		id_t create_capsule_y(f32 const radius, f32 const height, btVector3 const& origin, f32 const mass);
		id_t create_capsule_z(f32 const radius, f32 const height, btVector3 const& origin, f32 const mass);
		std::vector<id_t> create_from_phorm(sptr<phorm> const& phorm);
		void update(f32 const dt);
	private:
		optr<btDefaultCollisionConfiguration> m_config;
		optr<btCollisionDispatcher> m_dispatcher;
		optr<btBroadphaseInterface> m_cache;
		optr<btSequentialImpulseConstraintSolver> m_solver;
		optr<btDiscreteDynamicsWorld> m_world;
	};
} // namespace djinn

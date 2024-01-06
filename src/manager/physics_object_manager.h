#pragma once
#include "pch.h"
#include "manager.h"
#include "scene/physics_object.h"

namespace djinn
{
	class physics_object_manager final : public ref_counted_manager<physics_object>
	{
	public:
		physics_object_manager();
		DCM(physics_object_manager);
		~physics_object_manager();
	public:
		id_t create(btVector3 const& dims, btVector3 const& origin, f32 const mass);
		void destroy(id_t const id) override;
		void update(f32 const dt);
	private:
		optr<btDefaultCollisionConfiguration> m_config;
		optr<btCollisionDispatcher> m_dispatcher;
		optr<btBroadphaseInterface> m_cache;
		optr<btSequentialImpulseConstraintSolver> m_solver;
		optr<btDiscreteDynamicsWorld> m_world;
	};
} // namespace djinn

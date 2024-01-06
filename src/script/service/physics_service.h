#pragma once
#include "pch.h"
#include "service.h"
#include "manager/physics_object_manager.h"

namespace djinn
{
	class physics_service final : public haul::parent<service<physics_service>>
	{
		friend super;
	public:
		DCM(physics_service);
		~physics_service();
	public:
		static void init();
		static void shutdown();
		static void register_functions(JSContext* const ctx);
		static void update(f32 const dt);
	public:
		static physics_object_manager* get_physics_object_manager();
	private:
		physics_object_manager m_physics_object_manager;
	private:
		physics_service();
	};
} // namespace djinn

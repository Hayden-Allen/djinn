#pragma once
#include "pch.h"
#include "service.h"
#include "script/manager/entity_manager.h"

namespace djinn
{
	class scene_service final : public haul::parent<service<scene_service>>
	{
		friend class super;
	public:
		DCM(scene_service);
	public:
		static void init();
		static void shutdown();
		static void register_functions(JSContext* const ctx);
		static entity_manager* get_entity_manager();
	private:
		JSRuntime* const m_runtime;
		entity_manager m_entity_manager;
	private:
		scene_service();
	};
} // namespace djinn

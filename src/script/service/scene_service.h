#pragma once
#include "pch.h"
#include "service.h"
#include "script/manager/entity_manager.h"
#include "script/manager/camera_entity_manager.h"

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
		static camera_entity_manager* get_camera_entity_manager();
		static JSRuntime* get_runtime();
		static void update_all(f32 const dt);
		static void draw_all();
		static void draw_ui_all();
	private:
		JSRuntime* const m_runtime;
		entity_manager m_entity_manager;
		camera_entity_manager m_camera_entity_manager;
	private:
		scene_service();
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "service.h"
#include "script/manager/entity_manager.h"
#include "script/manager/camera_entity_manager.h"
#include "script/manager/mesh_instance_manager.h"

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
		static void free_all_entities();
		static void register_functions(JSContext* const ctx);
		static entity_manager* get_entity_manager();
		static camera_entity_manager* get_camera_entity_manager();
		static mesh_instance_manager* get_mesh_instance_manager();
		static JSRuntime* get_runtime();
		static void update(f32 const dt);
		static void draw();
		static void draw_ui();
		static void draw_imgui();
	private:
		JSRuntime* const m_runtime;
		entity_manager m_entity_manager;
		camera_entity_manager m_camera_entity_manager;
		mesh_instance_manager m_mesh_instance_manager;
	private:
		scene_service();
	};
} // namespace djinn

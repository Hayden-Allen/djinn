#pragma once
#include "pch.h"
#include "service.h"
#include "manager/scene/entity_manager.h"
#include "manager/scene/camera_entity_manager.h"
#include "manager/scene/mesh_instance_manager.h"
#include "manager/scene/physics_object_manager.h"
#include "manager/xport/phorm_manager.h"
#include "manager/xport/light_manager.h"
#include "manager/xport/waypoint_manager.h"
#include "manager/scene/sound_emitter_manager.h"

namespace djinn
{
	class scene_service final : public haul::parent<service<scene_service>>
	{
		friend super;
	public:
		DCM(scene_service);
	public:
		static void init();
		static void shutdown();
		static void free_all();
		static void register_functions(JSContext* const ctx);
		static entity_manager* get_entity_manager();
		static camera_entity_manager* get_camera_entity_manager();
		static mesh_instance_manager* get_mesh_instance_manager();
		static physics_object_manager* get_physics_object_manager();
		static phorm_manager* get_phorm_manager();
		static light_manager* get_light_manager();
		static waypoint_manager* get_waypoint_manager();
		static sound_emitter_manager* get_sound_emitter_manager();
		static JSRuntime* get_runtime();
		static void update(f32 const dt, f32 const time);
		static void draw();
		static void draw_ui();
		static void draw_imgui();
		static sptr<scene_object> get_scene_object(id_t const id);
		static tagged* get_tagged(id_t const id);
	private:
		JSRuntime* const m_runtime;
		entity_manager m_entity_manager;
		camera_entity_manager m_camera_entity_manager;
		mesh_instance_manager m_mesh_instance_manager;
		physics_object_manager m_physics_object_manager;
		phorm_manager m_phorm_manager;
		light_manager m_light_manager;
		waypoint_manager m_waypoint_manager;
		sound_emitter_manager m_emitter_manager;
	private:
		scene_service();
	};
} // namespace djinn

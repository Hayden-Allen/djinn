#pragma once
#include "pch.h"
#include "service.h"
#include "manager/custom_mesh_manager.h"
#include "manager/static_mesh_manager.h"
#include "manager/shader_manager.h"
#include "manager/texture_manager.h"
#include "manager/cubemap_manager.h"
#include "manager/sound_source_manager.h"

namespace djinn
{
	class asset_service final : public haul::parent<service<asset_service>>
	{
		friend super;
	public:
		DCM(asset_service);
	public:
		static void init();
		static void register_functions(JSContext* const ctx);
		static shader_manager* get_shader_manager();
		static texture_manager* get_texture_manager();
		static cubemap_manager* get_cubemap_manager();
		static custom_mesh_manager* get_custom_mesh_manager();
		static static_mesh_manager* get_static_mesh_manager();
		static sound_source_manager* get_sound_source_manager();
		static sptr<mesh> get_mesh(id_t const id);
		static void draw_meshes();
	private:
		shader_manager m_shader_manager;
		texture_manager m_texture_manager;
		cubemap_manager m_cubemap_manager;
		custom_mesh_manager m_custom_mesh_manager;
		static_mesh_manager m_static_mesh_manager;
		sound_source_manager m_sound_source_manager;
	private:
		asset_service();
	};
} // namespace djinn

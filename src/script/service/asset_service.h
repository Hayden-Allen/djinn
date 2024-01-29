#pragma once
#include "pch.h"
#include "service.h"
#include "manager/asset/custom_mesh_manager.h"
#include "manager/asset/loaded_mesh_manager.h"
#include "manager/asset/shader_manager.h"
#include "manager/asset/texture_manager.h"
#include "manager/asset/cubemap_manager.h"
#include "manager/asset/sound_source_manager.h"
#include "manager/xport/material_manager.h"

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
		static animated_mesh_manager* get_animated_mesh_manager();
		static sound_source_manager* get_sound_source_manager();
		static material_manager* get_material_manager();
		static sptr<mesh> get_mesh(id_t const id);
		static void update();
		static void set_default_texture_options(texture_options const& opts);
		static texture_options const& get_default_texture_options();
	private:
		shader_manager m_shader_manager;
		texture_manager m_texture_manager;
		cubemap_manager m_cubemap_manager;
		custom_mesh_manager m_custom_mesh_manager;
		static_mesh_manager m_static_mesh_manager;
		animated_mesh_manager m_animated_mesh_manager;
		sound_source_manager m_sound_source_manager;
		material_manager m_material_manager;
		texture_options m_default_texture_options;
	private:
		asset_service();
	};
} // namespace djinn

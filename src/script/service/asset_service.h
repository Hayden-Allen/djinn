#pragma once
#include "pch.h"
#include "service.h"
#include "script/manager/mesh_manager.h"
#include "script/manager/shader_manager.h"
#include "script/manager/texture_manager.h"
#include "script/manager/cubemap_manager.h"
#include "script/manager/sound_source_manager.h"

namespace djinn
{
	class asset_service final : public haul::parent<service<asset_service>>
	{
		friend class super;
	public:
		DCM(asset_service);
	public:
		static void init();
		static void register_functions(JSContext* const ctx);
		static shader_manager* get_shader_manager();
		static texture_manager* get_texture_manager();
		static cubemap_manager* get_cubemap_manager();
		static mesh_manager* get_mesh_manager();
		static sound_source_manager* get_sound_source_manager();
	private:
		shader_manager m_shader_manager;
		texture_manager m_texture_manager;
		cubemap_manager m_cubemap_manager;
		mesh_manager m_mesh_manager;
		sound_source_manager m_sound_source_manager;
	private:
		asset_service();
	};
} // namespace djinn

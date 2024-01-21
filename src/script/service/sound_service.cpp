#include "pch.h"
#include "sound_service.h"
#include "script/js.h"
#include "core/constants.h"
#include "asset_service.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

namespace djinn::js::sound_service
{
	JSValue set_listener_world_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		point<space::WORLD> const& pos = js::extract_point<space::WORLD>(ctx, argv[0]);
		::djinn::sound_service::get()->set_listener_world_pos(pos);
		return JS_UNDEFINED;
	}
	JSValue set_listener_world_dir(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		direction<space::WORLD> const& dir = js::extract_direction<space::WORLD>(ctx, argv[0]);
		::djinn::sound_service::get()->set_listener_world_dir(dir);
		return JS_UNDEFINED;
	}
} // namespace djinn::js::sound_service



namespace djinn
{
	sound_service::~sound_service()
	{
		if (m_engine_ready)
		{
			ma_engine_uninit(&m_engine);
			m_engine_ready = false;
		}
	}



	void sound_service::init()
	{
		ASSERT(!s_instance);
		s_instance = new sound_service();
		asset_service::get_sound_source_manager()->configure();
		ma_engine_listener_set_world_up(sound_service::get_engine(), 0, 0, 1, 0);
	}
	void sound_service::shutdown()
	{
		asset_service::get_sound_source_manager()->unconfigure();
		super::shutdown();
	}
	void sound_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "setListenerWorldPos", 1, js::sound_service::set_listener_world_pos);
		super::register_function(ctx, "setListenerWorldDir", 1, js::sound_service::set_listener_world_dir);

		super::register_property_u32(ctx, "ATTENUATION_INVERSE", ma_attenuation_model_inverse);
		super::register_property_u32(ctx, "ATTENUATION_LINEAR", ma_attenuation_model_linear);
		super::register_property_u32(ctx, "ATTENUATION_EXPONENTIAL", ma_attenuation_model_exponential);
	}
	void sound_service::set_listener_world_pos(point<space::WORLD> const& pos)
	{
		ASSERT(s_instance && s_instance->m_engine_ready);
		ma_engine_listener_set_position(sound_service::get_engine(), 0, pos.x, pos.y, pos.z);
	}
	void sound_service::set_listener_world_dir(direction<space::WORLD> const& dir)
	{
		ASSERT(s_instance && s_instance->m_engine_ready);
		ma_engine_listener_set_direction(sound_service::get_engine(), 0, dir.x, dir.y, dir.z);
	}



	ma_engine* sound_service::get_engine()
	{
		ASSERT(s_instance);
		return s_instance->m_engine_ready ? &s_instance->m_engine : nullptr;
	}



	sound_service::sound_service() :
		haul::parent<service<sound_service>>("Sound")
	{
		ma_engine_config engine_cfg = ma_engine_config_init();
		if (ma_engine_init(&engine_cfg, &m_engine) == MA_SUCCESS)
		{
			m_engine_ready = true;
		}
		else
		{
			ASSERT(false);
		}
	}
} // namespace djinn

#include "pch.h"
#include "sound_service.h"
#include "script/js.h"
#include "core/constants.h"
#include "asset_service.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

namespace djinn::js::sound_service
{
	JSValue create_sound_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const sound_source_id = js::extract_id(ctx, argv[0]);
		sptr<sound_source> source = ::djinn::asset_service::get_sound_source_manager()->get(sound_source_id);
		id_t const instance_id = ::djinn::sound_service::get_sound_emitter_manager()->create(source);
		return js::create_id(ctx, instance_id);
	}
	JSValue destroy_sound_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t id = js::extract_id(ctx, argv[0]);
		::djinn::sound_service::get_sound_emitter_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue play_sound_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t id = js::extract_id(ctx, argv[0]);
		::djinn::sound_service::get_sound_emitter_manager()->get(id)->play();
		return JS_UNDEFINED;
	}
	JSValue stop_sound_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t id = js::extract_id(ctx, argv[0]);
		::djinn::sound_service::get_sound_emitter_manager()->get(id)->stop();
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
	}
	void sound_service::shutdown()
	{
		asset_service::get_sound_source_manager()->unconfigure();
		super::shutdown();
	}
	void sound_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "Emitter", "create", 1, js::sound_service::create_sound_emitter);
		super::register_function(ctx, "Emitter", "destroy", 1, js::sound_service::destroy_sound_emitter);
		super::register_function(ctx, "Emitter", "play", 1, js::sound_service::play_sound_emitter);
		super::register_function(ctx, "Emitter", "stop", 1, js::sound_service::stop_sound_emitter);
	}
	void sound_service::update()
	{
		//
	}



	ma_engine* sound_service::get_engine()
	{
		ASSERT(s_instance);
		return s_instance->m_engine_ready ? &s_instance->m_engine : nullptr;
	}
	sound_emitter_manager* sound_service::get_sound_emitter_manager()
	{
		ASSERT(s_instance);
		return &s_instance->m_sound_emitter_manager;
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

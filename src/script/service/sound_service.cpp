#include "pch.h"
#include "sound_service.h"
#include "script/js.h"
#include "core/constants.h"
#include "asset_service.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

namespace djinn::js::sound_service
{
	JSValue create_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const sound_source_id = js::extract_id(ctx, argv[0]);
		sptr<sound_source> source = ::djinn::asset_service::get_sound_source_manager()->get(sound_source_id);
		id_t const instance_id = ::djinn::sound_service::get_sound_emitter_manager()->create(source);
		return js::create_id(ctx, instance_id);
	}
	JSValue destroy_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::sound_service::get_sound_emitter_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue start_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::sound_service::get_sound_emitter_manager()->get(id)->start();
		return JS_UNDEFINED;
	}
	JSValue stop_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::sound_service::get_sound_emitter_manager()->get(id)->stop();
		return JS_UNDEFINED;
	}
	JSValue set_emitter_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv) {
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const pos = js::extract_f32_array(ctx, argv[1]);
		::djinn::sound_service::get_sound_emitter_manager()->get(id)->set_pos(pos[0], pos[1], pos[2]);
		return JS_UNDEFINED;
	}
	JSValue set_emitter_volume(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv) {
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const volume = js::extract_f32(ctx, argv[1]);
		::djinn::sound_service::get_sound_emitter_manager()->get(id)->set_volume(volume);
		return JS_UNDEFINED;
	}
	JSValue set_emitter_spatialization_enabled(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv) {
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		bool const enabled = js::extract_bool(ctx, argv[1]);
		::djinn::sound_service::get_sound_emitter_manager()->get(id)->set_spatialization_enabled(enabled);
		return JS_UNDEFINED;
	}
	JSValue set_emitter_looping(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv) {
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		bool const looping = js::extract_bool(ctx, argv[1]);
		::djinn::sound_service::get_sound_emitter_manager()->get(id)->set_looping(looping);
		return JS_UNDEFINED;
	}
	JSValue set_listener_world_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv) {
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const pos = js::extract_f32_array(ctx, argv[1]);
		::djinn::sound_service::get()->set_listener_world_pos(pos[0], pos[1], pos[2]);
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
		super::register_function(ctx, "Emitter", "create", 1, js::sound_service::create_emitter);
		super::register_function(ctx, "Emitter", "destroy", 1, js::sound_service::destroy_emitter);
		super::register_function(ctx, "Emitter", "start", 1, js::sound_service::start_emitter);
		super::register_function(ctx, "Emitter", "stop", 1, js::sound_service::stop_emitter);
		super::register_function(ctx, "Emitter", "setPos", 2, js::sound_service::set_emitter_pos);
		super::register_function(ctx, "Emitter", "setVolume", 2, js::sound_service::set_emitter_volume);
		super::register_function(ctx, "Emitter", "setSpatializationEnabled", 2, js::sound_service::set_emitter_spatialization_enabled);
		super::register_function(ctx, "Emitter", "setLooping", 2, js::sound_service::set_emitter_looping);
		super::register_function(ctx, "setListenerWorldPos", 2, js::sound_service::set_listener_world_pos);
	}
	void sound_service::update()
	{
		s_instance->m_sound_emitter_manager.for_each([](sptr<sound_emitter> emitter, id_t const id)
			{
				emitter->update_from_scene_object();
			});
	}
	void sound_service::set_listener_world_pos(f32 const x, f32 const y, f32 const z)
	{
		ASSERT(s_instance && s_instance->m_engine_ready);
		ma_engine_listener_set_position(sound_service::get_engine(), 0, x, y, z);
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

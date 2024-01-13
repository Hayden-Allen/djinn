#include "pch.h"
#include "sound_service.h"
#include "script/js.h"
#include "core/constants.h"
#include "asset_service.h"
#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

namespace djinn::js::sound_service
{
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

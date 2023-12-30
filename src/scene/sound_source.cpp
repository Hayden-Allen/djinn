#include "pch.h"
#include "sound_source.h"
#include "sound_emitter.h"
#include "script/service/asset_service.h"

namespace djinn
{
	sound_source::sound_source()
	{
	}
	sound_source::~sound_source()
	{
		ASSERT(m_source_ready);
		ma_resource_manager_data_source_uninit(&m_source);
	}


	
	void sound_source::load(const std::string& afp)
	{
		for (auto emitter : m_emitters)
		{
			emitter->before_reload();
		}

		if (m_source_ready)
		{
			ma_resource_manager_data_source_uninit(&m_source);
			m_source_ready = false;
		}

		ma_resource_manager* resource_mgr = asset_service::get_sound_source_manager()->get_resource_manager();
		if (!resource_mgr)
		{
			ASSERT(false); // FIXME: This happens when the sound service is disabled, we need to figure out how to handle this
		}

		ma_result result = ma_resource_manager_data_source_init(resource_mgr, afp.c_str(), 0, nullptr, &m_source);
		ASSERT(result == MA_SUCCESS);

		m_source_ready = true;

		for (auto emitter : m_emitters)
		{
			emitter->after_reload();
		}
	}
	void sound_source::register_emitter(wptr<sound_emitter> emitter)
	{
		m_emitters.insert(emitter);
	}
	void sound_source::unregister_emitter(wptr<sound_emitter> emitter)
	{
		m_emitters.erase(emitter);
	}
	ma_resource_manager_data_source* sound_source::get_data_source()
	{
		return m_source_ready ? &m_source : nullptr;
	}
} // namespace djinn

#include "pch.h"
#include "sound_emitter.h"
#include "sound_source.h"
#include "script/service/sound_service.h"

namespace djinn
{
	sound_emitter::sound_emitter(id_t const id, sptr<sound_source> source) :
		scene_object(id),
		m_source(source)
	{
		reload_source();
	}
	sound_emitter::~sound_emitter()
	{
		m_source->unregister_emitter(wptr(this));
		ASSERT(m_sound_ready);
		ma_sound_uninit(&m_sound);
	}



	void sound_emitter::reload_source()
	{
		before_reload();
		if (m_sound_ready)
		{
			ma_sound_uninit(&m_sound);
		}
		ma_result result = ma_sound_init_from_data_source(sound_service::get_engine(), m_source->get_data_source(), 0, nullptr, &m_sound);
		ASSERT(result == MA_SUCCESS);
		after_reload();
		m_sound_ready = true;
	}



	void sound_emitter::play()
	{
		ma_sound_set_spatialization_enabled(&m_sound, false);
		ma_sound_set_volume(&m_sound, 1.0f);
		ma_sound_set_looping(&m_sound, true);
		ma_sound_start(&m_sound);
	}
	void sound_emitter::stop()
	{
		ma_sound_stop(&m_sound);
	}



	void sound_emitter::before_reload()
	{
		// TODO: Return backup of settings on m_sound before it is reset
	}
	void sound_emitter::after_reload()
	{
		// TODO: Restore backup of settings for m_sound after it has been reset
	}
} // namespace djinn

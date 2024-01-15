#include "pch.h"
#include "sound_emitter.h"
#include "asset/sound_source.h"
#include "script/service/sound_service.h"

namespace djinn
{
	sound_emitter::sound_emitter(id_t const id, sptr<sound_source> source) :
		scene_object_base(id),
		m_source(source)
	{
		before_reload();
		after_reload();
	}
	sound_emitter::~sound_emitter()
	{
		ASSERT(m_sound_ready);
		m_source->unregister_emitter(wptr(this));
		ma_sound_uninit(&m_sound);
	}



	void sound_emitter::before_reload()
	{
		if (m_sound_ready)
		{
			backup();
			ma_sound_stop(&m_sound);
			ma_sound_uninit(&m_sound);
			m_sound_ready = false;
		}
	}
	void sound_emitter::after_reload()
	{
		ma_result result = ma_sound_init_from_data_source(sound_service::get_engine(), m_source->get_data_source(), 0, nullptr, &m_sound);
		ASSERT(result == MA_SUCCESS);
		m_sound_ready = true;
		if (m_backup.valid)
		{
			restore();
		}
	}



	void sound_emitter::play()
	{
		ASSERT(m_sound_ready);
		ma_sound_set_spatialization_enabled(&m_sound, false);
		ma_sound_set_volume(&m_sound, 1.0f);
		ma_sound_set_looping(&m_sound, true);
		ma_sound_start(&m_sound);
	}
	void sound_emitter::stop()
	{
		ASSERT(m_sound_ready);
		ma_sound_stop(&m_sound);
	}



	void sound_emitter::backup()
	{
		ASSERT(m_sound_ready);
		m_backup.playing = ma_sound_is_playing(&m_sound);
		m_backup.valid = true;
	}
	void sound_emitter::restore()
	{
		ASSERT(m_backup.valid);
		if (m_backup.playing)
		{
			play();
		}
		m_backup.valid = false;
	}
} // namespace djinn

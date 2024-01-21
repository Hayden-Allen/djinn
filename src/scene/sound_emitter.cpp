#include "pch.h"
#include "sound_emitter.h"
#include "asset/sound_source.h"
#include "script/service/sound_service.h"

namespace djinn
{
	sound_emitter::sound_emitter(id_t const id, sptr<sound_source> source) :
		scene_object(id),
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
		update_from_scene_object();
		if (m_backup.valid)
		{
			restore();
		}
		else
		{
			set_volume(1.0f);
			set_spatialization_enabled(true);
			set_looping(false);
		}
	}



	void sound_emitter::start()
	{
		ASSERT(m_sound_ready);
		ma_sound_start(&m_sound);
	}
	void sound_emitter::stop()
	{
		ASSERT(m_sound_ready);
		ma_sound_stop(&m_sound);
	}
	void sound_emitter::set_volume(f32 const volume)
	{
		ASSERT(m_sound_ready);
		ma_sound_set_volume(&m_sound, volume);
	}
	void sound_emitter::set_spatialization_enabled(bool const enabled)
	{
		ASSERT(m_sound_ready);
		ma_sound_set_spatialization_enabled(&m_sound, enabled);
	}
	void sound_emitter::set_looping(bool const looping)
	{
		ASSERT(m_sound_ready);
		ma_sound_set_looping(&m_sound, looping);
	}
	void sound_emitter::update_from_scene_object()
	{
		ASSERT(m_sound_ready);
		tmat<space::OBJECT, space::WORLD> const mat = get_world_transform();
		ma_sound_set_position(&m_sound, mat.t[0], mat.t[1], mat.t[2]);
	}



	void sound_emitter::backup()
	{
		ASSERT(m_sound_ready);
		m_backup.volume = ma_sound_get_volume(&m_sound);
		m_backup.spatialization_enabled = ma_sound_is_spatialization_enabled(&m_sound);
		m_backup.looping = ma_sound_is_looping(&m_sound);
		m_backup.started = ma_sound_is_playing(&m_sound);
		m_backup.valid = true;
	}
	void sound_emitter::restore()
	{
		ASSERT(m_backup.valid);
		set_volume(m_backup.volume);
		set_spatialization_enabled(m_backup.spatialization_enabled);
		set_looping(m_backup.looping);
		if (m_backup.started)
		{
			start();
		}
		else
		{
			stop();
		}
		m_backup.valid = false;
	}
} // namespace djinn

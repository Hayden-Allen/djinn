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
		m_source->add_emitter(wptr(this));
		before_reload();
		after_reload();
	}
	sound_emitter::~sound_emitter()
	{
		ASSERT(m_sound_ready);
		m_source->remove_emitter(wptr(this));
		ma_sound_uninit(&m_sound);
	}



	void sound_emitter::update_from_scene_object()
	{
		ASSERT(m_sound_ready);
		tmat<space::OBJECT, space::WORLD> const mat = get_world_transform();
		ma_sound_set_position(&m_sound, mat.t[0], mat.t[1], mat.t[2]);
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
	void sound_emitter::set_rolloff(f32 const rolloff)
	{
		ASSERT(m_sound_ready);
		ma_sound_set_rolloff(&m_sound, rolloff);
	}
	void sound_emitter::set_min_distance(f32 const min)
	{
		ASSERT(m_sound_ready);
		ma_sound_set_min_distance(&m_sound, min);
	}
	void sound_emitter::set_max_distance(f32 const max)
	{
		ASSERT(m_sound_ready);
		ma_sound_set_max_distance(&m_sound, max);
	}
	void sound_emitter::set_attenuation_model(ma_attenuation_model const model)
	{
		ASSERT(m_sound_ready);
		ma_sound_set_attenuation_model(&m_sound, model);
	}
	void sound_emitter::set_fade(f32 const from, f32 const to, u32 const ms)
	{
		ASSERT(from == -1 || from >= 0 && from <= 1);
		ASSERT(to == -1 || to >= 0 && to <= 1);
		ma_sound_set_fade_in_milliseconds(&m_sound, from, to, (u64)ms);
	}



	void sound_emitter::backup()
	{
		ASSERT(m_sound_ready);
		m_backup.volume = ma_sound_get_volume(&m_sound);
		m_backup.rolloff = ma_sound_get_rolloff(&m_sound);
		m_backup.min_dist = ma_sound_get_min_distance(&m_sound);
		m_backup.max_dist = ma_sound_get_max_distance(&m_sound);
		m_backup.attenuation = ma_sound_get_attenuation_model(&m_sound);
		m_backup.spatialization_enabled = ma_sound_is_spatialization_enabled(&m_sound);
		m_backup.looping = ma_sound_is_looping(&m_sound);
		m_backup.started = ma_sound_is_playing(&m_sound);
		m_backup.valid = true;
	}
	void sound_emitter::restore()
	{
		ASSERT(m_backup.valid);
		set_volume(m_backup.volume);
		set_rolloff(m_backup.rolloff);
		set_min_distance(m_backup.min_dist);
		set_max_distance(m_backup.max_dist);
		set_attenuation_model(m_backup.attenuation);
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

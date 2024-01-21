#pragma once
#include "pch.h"
#include "scene_object.h"

namespace djinn
{
	class sound_source;

	class sound_emitter final : public scene_object
	{
	private:
		struct sound_emitter_backup
		{
			f32 volume = 0.0f;
			bool spatialization_enabled = false;
			bool looping = false;
			bool started = false;
			bool valid = false;
		};
	public:
		sound_emitter(id_t const id, sptr<sound_source> source);
		~sound_emitter();
		DCM(sound_emitter);
	public:
		void before_reload();
		void after_reload();
	public:
		void start();
		void stop();
		void set_volume(f32 const volume);
		void set_spatialization_enabled(bool const enabled);
		void set_looping(bool const looping);
		void update_from_scene_object();
	private:
		sptr<sound_source> m_source;
		ma_sound m_sound;
		sound_emitter_backup m_backup;
		bool m_sound_ready = false;
	private:
		void backup();
		void restore();
	};
} // namespace djinn

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
			bool playing = false;
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
		void play();
		void stop();
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

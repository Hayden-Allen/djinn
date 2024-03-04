#pragma once
#include "pch.h"

namespace djinn
{
	class sound_emitter;

	class sound_source final
	{
	public:
		sound_source();
		~sound_source();
		DCM(sound_source);
	public:
		void load(std::string const& afp);
		void add_emitter(wptr<sound_emitter> emitter);
		void remove_emitter(wptr<sound_emitter> emitter);
		ma_resource_manager_data_source* get_data_source();
	private:
		std::unordered_set<sound_emitter*> m_emitters;
		ma_resource_manager_data_source m_source;
		bool m_source_ready = false;
	};
} // namespace djinn

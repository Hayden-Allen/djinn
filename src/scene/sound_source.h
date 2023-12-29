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
		void load(const std::string& afp);
		void register_emitter(wptr<sound_emitter> emitter);
		void unregister_emitter(wptr<sound_emitter> emitter);
		ma_resource_manager_data_source* get_data_source();
	private:
		std::unordered_set<wptr<sound_emitter>> m_emitters;
		ma_resource_manager_data_source m_source;
		bool m_source_ready = false;
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "service.h"

namespace djinn
{
	class sound_service final : public haul::parent<service<sound_service>>
	{
		friend super;
	public:
		DCM(sound_service);
		~sound_service();
	public:
		static void init();
		static void shutdown();
		static void register_functions(JSContext* const ctx);
		static void update();
	public:
		static void set_listener_world_pos(f32 const x, f32 const y, f32 const z);
	public:
		static ma_engine* get_engine();
	private:
		ma_engine m_engine;
		bool m_engine_ready = false;
	private:
		sound_service();
	};
} // namespace djinn

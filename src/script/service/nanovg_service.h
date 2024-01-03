#pragma once
#include "pch.h"
#include "service.h"

namespace djinn
{
	class nanovg_service final : public haul::parent<service<nanovg_service>>
	{
		friend super;
	public:
		DCM(nanovg_service);
		~nanovg_service();
	public:
		static void init();
		static void begin_frame(u32 const window_width, u32 const window_height);
		static void end_frame();
		static void register_functions(JSContext* const ctx);
		static NVGcontext* const get_context();
	private:
		NVGcontext* const m_context;
		u32 m_width, m_height;
	private:
		nanovg_service();
	};
} // namespace djinn

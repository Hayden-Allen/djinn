#pragma once
#include "pch.h"
#include "service.h"

namespace djinn
{
	namespace js::nanovg_service
	{
		extern JSValue fill_style(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue fill_rect(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue fill_arc(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue fill_circle(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue stroke_style(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue stroke_line(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue stroke_rect(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue stroke_arc(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue stroke_circle(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue draw_basis(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
	} // namespace js::nanovg_service

	class nanovg_service final : public haul::parent<service<nanovg_service>>
	{
		friend class super;
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

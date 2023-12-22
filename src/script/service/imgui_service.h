#pragma once
#include "pch.h"
#include "service.h"

namespace djinn
{
	class imgui_service final : public haul::parent<service<imgui_service>>
	{
		friend class super;
	public:
		DCM(imgui_service);
	public:
		static void init(sptr<mgl::context> ctx);
		static void shutdown();
		static void register_functions(JSContext* const ctx);
		static void begin_frame();
		static void end_frame();
	private:
		sptr<mgl::context> m_context;
		f32 m_prev_x_scale, m_prev_y_scale, m_font_size;
	private:
		imgui_service(sptr<mgl::context> ctx);
	};
} // namespace djinn

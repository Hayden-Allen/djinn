#include "pch.h"
#include "nanovg_service.h"
#include "script/js.h"
#include "core/constants.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

namespace djinn
{
	namespace js::nanovg_service
	{
		template<typename FN>
		static JSValue set_style(FN const& fn, JSContext* const ctx, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 3 || argc == 4);
			f32 const r = js::extract_f32(ctx, argv[0]);
			f32 const g = js::extract_f32(ctx, argv[1]);
			f32 const b = js::extract_f32(ctx, argv[2]);
			f32 const a = (argc == 4 ? js::extract_f32(ctx, argv[3]) : 1.f);
			NVGcolor color = { r, g, b, a };
			NVGcontext* const nvg = ::djinn::nanovg_service::get_context();
			fn(nvg, color);
			return JS_UNDEFINED;
		}
		template<typename FN>
		static JSValue draw_rect(FN const& fn, JSContext* const ctx, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 4);
			f32 const x = js::extract_f32(ctx, argv[0]);
			f32 const y = js::extract_f32(ctx, argv[1]);
			f32 const w = js::extract_f32(ctx, argv[2]);
			f32 const h = js::extract_f32(ctx, argv[3]);
			NVGcontext* const nvg = ::djinn::nanovg_service::get_context();
			nvgBeginPath(nvg);
			nvgRect(nvg, x, y, w, h);
			fn(nvg);
			return JS_UNDEFINED;
		}
		static void draw_line(f32 const x0, f32 const y0, f32 const x1, f32 const y1)
		{
			NVGcontext* const nvg = ::djinn::nanovg_service::get_context();
			nvgBeginPath(nvg);
			nvgMoveTo(nvg, x0, y0);
			nvgLineTo(nvg, x1, y1);
			nvgStroke(nvg);
		}
		template<typename FN>
		static JSValue draw_arc(FN const& fn, JSContext* const ctx, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 5 || argc == 6);
			f32 const x = js::extract_f32(ctx, argv[0]);
			f32 const y = js::extract_f32(ctx, argv[1]);
			f32 const r = js::extract_f32(ctx, argv[2]);
			f32 const start = js::extract_f32(ctx, argv[3]);
			f32 const end = js::extract_f32(ctx, argv[4]);
			u32 const dir = (argc == 6 ? js::extract_u32(ctx, argv[5]) : NVG_CCW);
			NVGcontext* const nvg = ::djinn::nanovg_service::get_context();
			nvgBeginPath(nvg);
			nvgArc(nvg, x, y, r, start, end, dir);
			fn(nvg);
			return JS_UNDEFINED;
		}
		template<typename FN>
		static JSValue draw_circle(FN const& fn, JSContext* const ctx, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 3);
			f32 const x = js::extract_f32(ctx, argv[0]);
			f32 const y = js::extract_f32(ctx, argv[1]);
			f32 const r = js::extract_f32(ctx, argv[2]);
			NVGcontext* const nvg = ::djinn::nanovg_service::get_context();
			nvgBeginPath(nvg);
			nvgCircle(nvg, x, y, r);
			fn(nvg);
			return JS_UNDEFINED;
		}



		JSValue fill_style(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			return set_style(nvgFillColor, ctx, argc, argv);
		}
		JSValue fill_rect(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			return draw_rect(nvgFill, ctx, argc, argv);
		}
		JSValue fill_arc(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			return draw_arc(nvgFill, ctx, argc, argv);
		}
		JSValue fill_circle(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			return draw_circle(nvgFill, ctx, argc, argv);
		}
		JSValue stroke_style(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			return set_style(nvgStrokeColor, ctx, argc, argv);
		}
		JSValue stroke_line(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 4);
			f32 const x0 = js::extract_f32(ctx, argv[0]);
			f32 const y0 = js::extract_f32(ctx, argv[1]);
			f32 const x1 = js::extract_f32(ctx, argv[2]);
			f32 const y1 = js::extract_f32(ctx, argv[3]);
			draw_line(x0, y0, x1, y1);
			return JS_UNDEFINED;
		}
		JSValue stroke_rect(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			return draw_rect(nvgStroke, ctx, argc, argv);
		}
		JSValue stroke_arc(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			return draw_arc(nvgStroke, ctx, argc, argv);
		}
		JSValue stroke_circle(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			return draw_circle(nvgStroke, ctx, argc, argv);
		}
		JSValue draw_basis(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(false);
			// ASSERT(argc == 2);
			// id_t const cam_id = js::extract_id(ctx, argv[0]);
			// id_t const tgt_id = js::extract_id(ctx, argv[1]);
			// sptr<camera> cam = render_service::get_camera_manager().get(cam_id);
			// sptr<entity> tgt = scene_service::get_entity_manager().get(tgt_id);
			// tmat<space::OBJECT, space::WORLD> const& obj2world = tgt->get_world();
			// tmat<space::WORLD, space::CLIP> const& world2clip = cam->get_view_proj();
			// tmat<space::OBJECT, space::CLIP> const& obj2clip = world2clip * obj2world;
			// vec<space::DEVICE> const& i = obj2clip.get_i();
			// vec<space::DEVICE> const& j = obj2clip.get_j();
			// vec<space::DEVICE> const& k = obj2clip.get_k();
			// point<space::DEVICE> const& t = obj2clip.get_t().perspective_divide();
			// u32 const w = ::djinn::nanovg_service::get_width();
			// u32 const h = ::djinn::nanovg_service::get_height();
			// vec<space::VIEWPORT> const& vi = i.to_viewport(w, h);
			// vec<space::VIEWPORT> const& vj = j.to_viewport(w, h);
			// vec<space::VIEWPORT> const& vk = k.to_viewport(w, h);
			// point<space::VIEWPORT> const& vt = t.to_viewport(w, h);
			// NVGcontext* const nvg = ::djinn::nanovg_service::get_context();
			// nvgStrokeColor(nvg, { 1, 0, 0, 1 }); draw_line(vt.x, vt.y, vt.x + vi.x, vt.y + vi.y);
			// nvgStrokeColor(nvg, { 0, 1, 0, 1 }); draw_line(vt.x, vt.y, vt.x + vj.x, vt.y + vj.y);
			// nvgStrokeColor(nvg, { 0, 0, 1, 1 }); draw_line(vt.x, vt.y, vt.x + vk.x, vt.y + vk.y);

			return JS_UNDEFINED;
		}
		JSValue set_font(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 2);
			std::string const& face = js::extract_string(ctx, argv[0]);
			f32 const size = js::extract_f32(ctx, argv[1]);
			NVGcontext* const nvg = ::djinn::nanovg_service::get_context();
			nvgFontSize(nvg, size);
			nvgFontFace(nvg, face.c_str());
			return JS_UNDEFINED;
		}
		JSValue draw_text(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 3);
			f32 const x = js::extract_f32(ctx, argv[0]);
			f32 const y = js::extract_f32(ctx, argv[1]);
			std::string const& msg = js::extract_string(ctx, argv[2]);
			NVGcontext* const nvg = ::djinn::nanovg_service::get_context();
			nvgBeginPath(nvg);
			nvgFontSize(nvg, 17.0f);
			nvgFontFace(nvg, "sans");
			nvgText(nvg, x, y, msg.c_str(), nullptr);
			return JS_UNDEFINED;
		}
	} // namespace js::nanovg_service



	nanovg_service::~nanovg_service()
	{
		nvgDeleteGL3(m_context);
	}



	void nanovg_service::init()
	{
		ASSERT(!s_instance);
		s_instance = new nanovg_service();
		NVGcontext* const nvg = s_instance->m_context;
		nvgCreateFont(nvg, "sans", (c::base_dir::font + std::string("/Roboto-Regular.ttf")).c_str());
	}
	void nanovg_service::begin_frame(u32 const window_width, u32 const window_height)
	{
		s_instance->m_width = window_width;
		s_instance->m_height = window_height;
		f32 const ar = 1.f * window_width / window_height;
		nvgBeginFrame(s_instance->m_context, 1.f * window_width, 1.f * window_height, ar);
	}
	void nanovg_service::end_frame()
	{
		nvgEndFrame(s_instance->m_context);
	}
	void nanovg_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "fillStyle", 4, js::nanovg_service::fill_style);
		super::register_function(ctx, "fillRect", 4, js::nanovg_service::fill_rect);
		super::register_function(ctx, "fillArc", 6, js::nanovg_service::fill_arc);
		super::register_function(ctx, "fillCircle", 3, js::nanovg_service::fill_circle);
		super::register_function(ctx, "strokeStyle", 4, js::nanovg_service::stroke_style);
		super::register_function(ctx, "strokeLine", 4, js::nanovg_service::stroke_line);
		super::register_function(ctx, "strokeRect", 4, js::nanovg_service::stroke_rect);
		super::register_function(ctx, "strokeArc", 6, js::nanovg_service::stroke_arc);
		super::register_function(ctx, "strokeCircle", 3, js::nanovg_service::stroke_circle);
		super::register_function(ctx, "setFont", 2, js::nanovg_service::set_font);
		super::register_function(ctx, "drawText", 3, js::nanovg_service::draw_text);
	}
	NVGcontext* const nanovg_service::get_context()
	{
		return s_instance->m_context;
	}



	nanovg_service::nanovg_service() :
		haul::parent<service<nanovg_service>>("Nanovg"),
		m_context(nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG)),
		m_width(0),
		m_height(0)
	{}
} // namespace djinn

#include "pch.h"
#include "render_service.h"
#include "asset_service.h"
#include "script/js.h"

namespace djinn
{
	namespace js::render_service
	{
		JSValue draw(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			/*ASSERT(argc == 2);
			id_t const ro_id = js::extract_id(ctx, argv[0]);
			id_t const shader_id = js::extract_id(ctx, argv[1]);
			sptr<static_render_object> ro = ::djinn::asset_service::get_mesh_manager()->get(ro_id);
			sptr<shaders> shaders = ::djinn::asset_service::get_shader_manager()->get(shader_id);
			::djinn::render_service::get_context()->draw(*ro.get(), *shaders.get());
			return JS_UNDEFINED;*/
			ASSERT(false);
			return JS_UNDEFINED;
		}
		JSValue bind_texture(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 2);
			id_t const id = js::extract_id(ctx, argv[0]);
			u32 const slot = js::extract_u32(ctx, argv[1]);
			::djinn::asset_service::get_texture_manager()->bind(id, slot);
			return JS_UNDEFINED;
		}
		JSValue bind_cubemap(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 2);
			id_t const id = js::extract_id(ctx, argv[0]);
			u32 const slot = js::extract_u32(ctx, argv[1]);
			::djinn::asset_service::get_cubemap_manager()->bind(id, slot);
			return JS_UNDEFINED;
		}
		JSValue get_aspect_ratio(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 0);
			return js::create_f32(ctx, ::djinn::render_service::get_context()->get_aspect_ratio());
		}
	} // namespace js::render_service



	void render_service::init(optr<mgl::context>& context)
	{
		ASSERT(!s_instance);
		s_instance = new render_service(context);
	}
	void render_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "draw", 2, js::render_service::draw);
		super::register_function(ctx, "bindTexture", 2, js::render_service::bind_texture);
		super::register_function(ctx, "bindCubemap", 2, js::render_service::bind_cubemap);
		super::register_function(ctx, "getAspectRatio", 0, js::render_service::get_aspect_ratio);
	}
	sptr<mgl::context> render_service::get_context()
	{
		return s_instance->m_context;
	}



	render_service::render_service(optr<mgl::context>& context) :
		haul::parent<service<render_service>>("Render"),
		m_context(context)
	{}
} // namespace djinn

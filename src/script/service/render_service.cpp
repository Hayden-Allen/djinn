#include "pch.h"
#include "render_service.h"
#include "asset_service.h"
#include "scene_service.h"
#include "script/js.h"

namespace djinn
{
	namespace js::render_service
	{
		JSValue get_aspect_ratio(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 0);
			return js::create_f32(ctx, ::djinn::render_service::get_context()->get_aspect_ratio());
		}
		JSValue set_debug_draw_enabled(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 1);
			bool const enabled = js::extract_bool(ctx, argv[0]);
			::djinn::render_service::set_debug_draw_enabled(enabled);
			return JS_UNDEFINED;
		}
	} // namespace js::render_service



	void render_service::init(optr<mgl::context>& context)
	{
		ASSERT(!s_instance);
		s_instance = new render_service(context);
	}
	void render_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "getAspectRatio", 0, js::render_service::get_aspect_ratio);
		super::register_function(ctx, "setDebugDrawEnabled", 1, js::render_service::set_debug_draw_enabled);

		super::register_property_u32(ctx, "GL_NEAREST", GL_NEAREST);
		super::register_property_u32(ctx, "GL_LINEAR", GL_LINEAR);
		super::register_property_u32(ctx, "GL_NEAREST_MIPMAP_NEAREST", GL_NEAREST_MIPMAP_NEAREST);
		super::register_property_u32(ctx, "GL_LINEAR_MIPMAP_NEAREST", GL_LINEAR_MIPMAP_NEAREST);
		super::register_property_u32(ctx, "GL_NEAREST_MIPMAP_LINEAR", GL_NEAREST_MIPMAP_LINEAR);
		super::register_property_u32(ctx, "GL_LINEAR_MIPMAP_LINEAR", GL_LINEAR_MIPMAP_LINEAR);
		super::register_property_u32(ctx, "GL_REPEAT", GL_REPEAT);
	}
	sptr<mgl::context> render_service::get_context()
	{
		ASSERT(s_instance);
		return s_instance->m_context;
	}
	void render_service::set_debug_draw_enabled(bool const enabled)
	{
		ASSERT(s_instance);
		s_instance->m_debug_draw_enabled = enabled;
	}
	bool render_service::is_debug_draw_enabled()
	{
		ASSERT(s_instance);
		return s_instance->m_debug_draw_enabled;
	}
	void render_service::draw_all()
	{
		asset_service::get_custom_mesh_manager()->for_each([](sptr<mesh> mesh, id_t const id)
			{
				mesh->draw(render_service::get_context());
			});
		asset_service::get_static_mesh_manager()->for_each([](sptr<mesh> mesh, id_t const id)
			{
				mesh->draw(render_service::get_context());
			});
		asset_service::get_animated_mesh_manager()->for_each([](sptr<mesh> mesh, id_t const id)
			{
				mesh->draw(render_service::get_context());
			});
		scene_service::get_phorm_manager()->draw_all();
	}



	render_service::render_service(optr<mgl::context>& context) :
		haul::parent<service<render_service>>("Render"),
		m_context(context),
		m_debug_draw_enabled(false)
	{}
} // namespace djinn

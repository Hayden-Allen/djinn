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
			ASSERT(argc == 2);
			id_t ro_id = js::extract_id(ctx, argv[0]);
			id_t shader_id = js::extract_id(ctx, argv[1]);
			sptr<static_render_object> ro = ::djinn::asset_service::get_mesh_manager()->get(ro_id);
			sptr<shaders> shaders = ::djinn::asset_service::get_shader_manager()->get(shader_id);
			::djinn::render_service::get_context()->draw(*ro.get(), *shaders.get());
			return JS_UNDEFINED;
		}
	} // namespace js::render_service



	void render_service::init(mgl::context* const context)
	{
		super::s_instance = new render_service(context);
	}
	void render_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "draw", 2, js::render_service::draw);
	}
	mgl::context* const render_service::get_context()
	{
		return s_instance->m_context;
	}



	render_service::render_service(mgl::context* const context) :
		haul::parent<service<render_service>>("Render"),
		m_context(context)
	{}
} // namespace djinn

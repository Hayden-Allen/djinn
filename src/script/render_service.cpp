#include "pch.h"
#include "render_service.h"
#include "asset_service.h"
#include "core/util.h"

namespace djinn
{
	namespace js::render_service
	{
		JSValue draw(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 2);
			id_t ro_id;
			if (!u::extract_u32(ctx, argv[0], &ro_id))
				return JS_EXCEPTION;
			id_t shader_id;
			if (!u::extract_u32(ctx, argv[1], &shader_id))
				return JS_EXCEPTION;
			sptr<static_render_object> ro = ::djinn::asset_service::get_instance()->get_mesh_manager().get(ro_id);
			sptr<shaders> shaders = ::djinn::asset_service::get_instance()->get_shader_manager().get(shader_id);
			::djinn::render_service::get_instance()->get_context()->draw(*ro.get(), *shaders.get());
			return JS_UNDEFINED;
		}
	}

	render_service::render_service(mgl::context* const context) :
		m_context(context)
	{}


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
		return m_context;
	}
} // namespace djinn

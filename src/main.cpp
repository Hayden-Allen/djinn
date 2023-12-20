#include "pch.h"
#include "debug/script_watcher.h"
#include "debug/shader_watcher.h"
#include "debug/texture_watcher.h"
#include "debug/cubemap_watcher.h"
#include "scene/script_manager.h"
#include "scene/entity.h"
#include "script/service/asset_service.h"
#include "script/service/render_service.h"
#include "script/service/nanovg_service.h"
#include "script/service/util_service.h"
#include "script/service/scene_service.h"
#include "script/service/input_service.h"
#include "core/constants.h"
#include "script/js.h"

using namespace djinn;

void script_main()
{
	std::string const& afp = u::to_absolute(djinn::c::base_dir::script, "main.js");
	std::string const& src = u::read_file(afp);
	printf("%s\n", src.c_str());

	JSContext* const ctx = JS_NewContext(scene_service::get_runtime());
	js::global::init_globals(ctx);
	asset_service::register_functions(ctx);
	render_service::register_functions(ctx);
	nanovg_service::register_functions(ctx);
	util_service::register_functions(ctx);
	scene_service::register_functions(ctx);
	input_service::register_functions(ctx);

	JSValue const eval_ret = JS_Eval(ctx, src.data(), src.size(), "main.js", JS_EVAL_TYPE_MODULE);
	if (JS_IsException(eval_ret))
	{
		JSValue const ex_val = JS_GetException(ctx);
		char const* const ex = JS_ToCString(ctx, ex_val);
		printf("Exception occurred: %s\n", ex);
		JS_FreeCString(ctx, ex);
		JS_FreeValue(ctx, ex_val);
	}
	JS_FreeValue(ctx, eval_ret);

	JSValue const global = JS_GetGlobalObject(ctx);
	JSValue const exports_def = JS_GetPropertyStr(ctx, global, "__djinnExports");
	JSValue const default_def = JS_GetPropertyStr(ctx, exports_def, "default");
	JSValue const call_ret = JS_Call(ctx, default_def, JS_UNDEFINED, 0, nullptr);
	if (JS_IsException(call_ret))
	{
		JSValue const ex_val = JS_GetException(ctx);
		char const* const ex = JS_ToCString(ctx, ex_val);
		printf("Exception occurred: %s\n", ex);
		JS_FreeCString(ctx, ex);
		JS_FreeValue(ctx, ex_val);
	}
	JS_FreeValue(ctx, call_ret);
	JS_FreeValue(ctx, default_def);
	JS_FreeValue(ctx, exports_def);
	JS_FreeValue(ctx, global);

	JS_FreeContext(ctx);
}

int main(int argc, char* argv[])
{
	optr<mgl::context> c(new mgl::context(1920, 1080, "mingl", { .vsync = true, .clear = { .b = 1 } }));
	asset_service::init();
	render_service::init(c);
	nanovg_service::init();
	util_service::init();
	scene_service::init();
	input_service::init(c);

	shader_watcher shader_watcher(asset_service::get_shader_manager());
	texture_watcher texture_watcher(asset_service::get_texture_manager());
	cubemap_watcher cubemap_watcher(asset_service::get_cubemap_manager());
	script_watcher script_watcher(scene_service::get_entity_manager());
	id_t const eid = scene_service::get_entity_manager()->load("test.js");
	// script_main();

	while (c->is_running())
	{
		// non blocking wait
		shader_watcher.poll();
		script_watcher.poll();
		texture_watcher.poll();
		cubemap_watcher.poll();

		c->begin_frame();
		c->clear();
		f32 const dt = c->time.delta;

		scene_service::get_entity_manager()->for_each([&](sptr<entity> e, id_t const id)
			{
				e->update(dt);
			});
		nanovg_service::begin_frame(c->get_width(), c->get_height());
		scene_service::get_entity_manager()->for_each([](sptr<entity> e, id_t const id)
			{
				e->draw();
			});
		scene_service::get_entity_manager()->for_each([](sptr<entity> e, id_t const id)
			{
				e->draw_ui();
			});
		nanovg_service::end_frame();
		glfwSwapBuffers(c->window);
	}

	render_service::shutdown();
	nanovg_service::shutdown();
	util_service::shutdown();
	scene_service::shutdown();
	input_service::shutdown();
	asset_service::shutdown();
	c.free();
	return 0;
}
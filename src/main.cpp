#include "pch.h"
#include "scene/entity.h"
#include "script/service/asset_service.h"
#include "script/service/render_service.h"
#include "script/service/nanovg_service.h"
#include "script/service/util_service.h"
#include "script/service/scene_service.h"
#include "script/service/input_service.h"
#include "script/service/imgui_service.h"
#include "core/constants.h"
#include "script/js.h"
#ifndef DJINN_DIST
#include "debug/script_watcher.h"
#include "debug/shader_watcher.h"
#include "debug/texture_watcher.h"
#include "debug/cubemap_watcher.h"
#include "scene/script_manager.h"
#endif

using namespace djinn;

int main(int argc, char* argv[])
{
	optr<mgl::context> c(new mgl::context(1920, 1080, "mingl", { .vsync = false, .clear = { .b = 1 } }));
	asset_service::init();
	render_service::init(c);
	nanovg_service::init();
	util_service::init();
	scene_service::init();
	input_service::init(c);
	imgui_service::init(c);

	#ifndef DJINN_DIST
	shader_watcher shader_watcher(asset_service::get_shader_manager());
	texture_watcher texture_watcher(asset_service::get_texture_manager());
	cubemap_watcher cubemap_watcher(asset_service::get_cubemap_manager());
	script_watcher_entity script_watcher_entity(scene_service::get_entity_manager());
	script_watcher_camera script_watcher_camera(scene_service::get_camera_entity_manager());
	#endif
	// for (int i = 0; i < 100; i++)
	scene_service::get_entity_manager()->load("test.js");
	// script_main();

	while (c->is_running())
	{
		// non blocking wait
		#ifndef DJINN_DIST
		shader_watcher.poll();
		script_watcher_entity.poll();
		script_watcher_camera.poll();
		texture_watcher.poll();
		cubemap_watcher.poll();
		#endif

		c->begin_frame();
		c->clear();
		char buf[128] = { 0 };
		sprintf_s(buf, "djinn - %dfps", (s32)c->avg_fps);
		glfwSetWindowTitle(c->window, buf);

		scene_service::update(c->time.delta);
		nanovg_service::begin_frame(c->get_width(), c->get_height());
		scene_service::draw();
		scene_service::draw_ui();
		nanovg_service::end_frame();
		#ifndef DJINN_DIST
		imgui_service::begin_frame();
		scene_service::draw_imgui();
		imgui_service::end_frame();
		#endif

		glfwSwapBuffers(c->window);
	}

	render_service::shutdown();
	nanovg_service::shutdown();
	util_service::shutdown();
	scene_service::shutdown();
	input_service::shutdown();
	asset_service::shutdown();
	imgui_service::shutdown();
	c.free();
	return 0;
}
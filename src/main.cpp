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
#	include "debug/script_watcher.h"
#	include "debug/shader_watcher.h"
#	include "debug/texture_watcher.h"
#	include "debug/cubemap_watcher.h"
#endif

#define DJINN_PROFILE 0
#if DJINN_PROFILE
#	define DJINN_TIME(code, var, factor)                                                                                                           \
		{                                                                                                                                           \
			auto start = std::chrono::high_resolution_clock::now();                                                                                 \
			code;                                                                                                                                   \
			var += 1.f * std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() / factor; \
		}
#else
#	define DJINN_TIME(code, var, factor) code;
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
	scene_service::get_entity_manager()->load("main.js");

#if DJINN_PROFILE
	u32 const NUM_FRAMES = 1000;
	f32 input_avg = 0, update_avg = 0, draw_avg = 0, ui_avg = 0, imgui_avg = 0;
	for (u32 i = 0; i < NUM_FRAMES; i++)
#else
	while (c->is_running())
#endif
	{
#ifndef DJINN_DIST
		// non blocking wait
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
		c->set_title(buf);

		DJINN_TIME(input_service::update(), input_avg, NUM_FRAMES);

		DJINN_TIME(scene_service::update(c->time.delta), update_avg, NUM_FRAMES);
		DJINN_TIME(scene_service::draw(), draw_avg, NUM_FRAMES);

		DJINN_TIME(
			nanovg_service::begin_frame(c->get_width(), c->get_height());
			scene_service::draw_ui();
			nanovg_service::end_frame();,
			ui_avg, NUM_FRAMES);

#ifndef DJINN_DIST
		DJINN_TIME(
			imgui_service::begin_frame();
			scene_service::draw_imgui();
			imgui_service::end_frame();,
			imgui_avg, NUM_FRAMES);
#endif

		glfwSwapBuffers(c->window);
	}
#if DJINN_PROFILE
	f32 avg_total = 1000 / c->avg_fps;
	printf("UPDATE: %f (%.2f%%)\n", update_avg, update_avg / avg_total * 100);
	printf("DRAW: %f (%.2f%%)\n", draw_avg, draw_avg / avg_total * 100);
	printf("UI: %f (%.2f%%)\n", ui_avg, ui_avg / avg_total * 100);
	printf("IMGUI: %f (%.2f%%)\n", imgui_avg, imgui_avg / avg_total * 100);
	printf("TOTAL: %f\n", avg_total);
#endif

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
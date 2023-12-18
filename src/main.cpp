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

using namespace djinn;

int main(int argc, char* argv[])
{
	optr<mgl::context> c(new mgl::context(1920, 1080, "mingl", { .vsync = true, .clear = { .b = 1 } }));
	asset_service::init();
	render_service::init(c);
	nanovg_service::init();
	util_service::init();
	scene_service::init();

	shader_watcher shader_watcher(asset_service::get_shader_manager());
	texture_watcher texture_watcher(asset_service::get_texture_manager());
	cubemap_watcher cubemap_watcher(asset_service::get_cubemap_manager());
	/*script_manager* g_all_scripts = new script_manager();
	script_watcher script_watcher(g_all_scripts);
	entity* const e = g_all_scripts->load("test.js");*/
	script_watcher script_watcher(scene_service::get_entity_manager());
	id_t const eid = scene_service::get_entity_manager()->load("test.js");
	sptr<entity> e = scene_service::get_entity_manager()->get(eid);



	f32 constexpr vertices[] = {
		-.5f,
		-.5f,
		0.f,
		0.f,
		0.f,
		.5f,
		-.5f,
		0.f,
		1.f,
		0.f,
		.5f,
		.5f,
		0.f,
		1.f,
		1.f,
		-.5f,
		.5f,
		0.f,
		0.f,
		1.f,
	};
	u32 indices[] = {
		0, 1, 2, 0, 2, 3
	};
	mgl::static_render_object ro(vertices, sizeof(vertices) / sizeof(f32), { 3, 2 }, indices, sizeof(indices) / sizeof(u32));
	mgl::shaders shaders = mgl::shaders("../../../../cwd/res/glsl/mingl/test.vert", "../../../../cwd/res/glsl/mingl/test.frag");
	shaders.uniform_1i("u_texture", 0);

	const u32 TW = 32, TH = 32;
	f32 pixels[TW * TH * 3] = { 0.f };
	for (int y = 0; y < TH; y++)
	{
		for (int x = 0; x < TW; x++)
		{
			const u32 off = y * (TW * 3) + x * 3;
			pixels[off + 0] = (x / (TW * 2.f) + y / (TH * 2.f));
			pixels[off + 1] = 0.f;
			pixels[off + 2] = 0.f;
		}
	}
	mgl::texture2d_rgb_f32 texture(GL_RGB, TW, TH, pixels, { .min_filter = GL_NEAREST, .mag_filter = GL_NEAREST });

	mgl::camera cam(point<space::WORLD>(0, 0, 5), 0, 0, 108 / c->get_aspect_ratio(), c->get_aspect_ratio(), .01f, 10.f, 1.f);

	constexpr u32 keycodes[] = { GLFW_KEY_W, GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_SHIFT, GLFW_KEY_ESCAPE, GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN };
	bool keys[11] = { false };
	f32 const speed = 1.f;



	while (c->is_running())
	{
		// non blocking wait
		shader_watcher.poll();
		script_watcher.poll();
		texture_watcher.poll();
		cubemap_watcher.poll();

		c->begin_frame();
		c->clear();

		e->update();
		nanovg_service::begin_frame(c->get_width(), c->get_height());
		e->draw();

		for (int i = 0; i < sizeof(keys) / sizeof(bool); i++)
			keys[i] = c->get_key(keycodes[i]);
		if (keys[6])
			break;

		const f32 fs = speed * c->time.delta;
		vec<space::CAMERA> cam_move(
			keys[3] - keys[1],
			keys[4] - keys[5],
			keys[2] - keys[0]);
		const f32 mx = 1.f * keys[8] - keys[7];
		const f32 my = 1.f * keys[10] - keys[9];
		cam.move(fs, cam_move, mx, my);

		mat<space::OBJECT, space::CLIP> const mvp = cam.get_view_proj() * mat<space::OBJECT, space::WORLD>();
		shaders.uniform_mat4("u_mvp", mvp.e);

		texture.bind(0);
		c->draw(ro, shaders);

		e->draw_ui();
		nanovg_service::end_frame();
		glfwSwapBuffers(c->window);
	}

	asset_service::shutdown();
	render_service::shutdown();
	c.free();
	return 0;
}
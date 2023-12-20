#include "pch.h"
#include "scene_service.h"
#include "script/js.h"
#include "core/constants.h"
#include "scene/camera_entity.h"

namespace djinn::js::scene_service
{
	JSValue load_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		return js::create_id(ctx, ::djinn::scene_service::get_entity_manager()->load(fp));
	}
	JSValue destroy_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_entity_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue load_camera(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		return js::create_id(ctx, ::djinn::scene_service::get_camera_entity_manager()->load(fp));
	}
	JSValue move_camera(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 7);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const dt = js::extract_f32(ctx, argv[1]);
		f32 const dx = js::extract_f32(ctx, argv[2]);
		f32 const dy = js::extract_f32(ctx, argv[3]);
		f32 const dz = js::extract_f32(ctx, argv[4]);
		f32 const mx = js::extract_f32(ctx, argv[5]);
		f32 const my = js::extract_f32(ctx, argv[6]);
		sptr<camera_entity> cam = ::djinn::scene_service::get_camera_entity_manager()->get(id);
		cam->move(dt, { dx, dy, dz }, mx, my);
		return JS_UNDEFINED;
	}
	JSValue configure_camera(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 5);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const fovy = js::extract_f32(ctx, argv[1]);
		f32 const aspect = js::extract_f32(ctx, argv[2]);
		f32 const near = js::extract_f32(ctx, argv[3]);
		f32 const far = js::extract_f32(ctx, argv[4]);
		sptr<camera_entity> cam = ::djinn::scene_service::get_camera_entity_manager()->get(id);
		cam->configure(fovy, aspect, near, far);
		return JS_UNDEFINED;
	}
	JSValue destroy_camera(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_camera_entity_manager()->destroy(id);
		return JS_UNDEFINED;
	}
} // namespace djinn::js::scene_service



namespace djinn
{
	void scene_service::init()
	{
		ASSERT(!s_instance);
		s_instance = new scene_service();
	}
	void scene_service::shutdown()
	{
		JSRuntime* const runtime = s_instance->m_runtime;
		super::shutdown();
		JS_FreeRuntime(runtime);
	}
	void scene_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "Entity", "load", 1, js::scene_service::load_entity);
		super::register_function(ctx, "Entity", "destroy", 1, js::scene_service::destroy_entity);
		super::register_function(ctx, "Camera", "load", 1, js::scene_service::load_camera);
		super::register_function(ctx, "Camera", "move", 7, js::scene_service::move_camera);
		super::register_function(ctx, "Camera", "configure", 5, js::scene_service::configure_camera);
		super::register_function(ctx, "Camera", "destroy", 1, js::scene_service::destroy_camera);
	}
	entity_manager* scene_service::get_entity_manager()
	{
		return &s_instance->m_entity_manager;
	}
	camera_entity_manager* scene_service::get_camera_entity_manager()
	{
		return &s_instance->m_camera_entity_manager;
	}
	JSRuntime* scene_service::get_runtime()
	{
		return s_instance->m_runtime;
	}
	void scene_service::update_all(f32 const dt)
	{
		s_instance->m_entity_manager.for_each([&](sptr<entity> e, id_t const id)
			{
				e->update(dt);
			});
		s_instance->m_camera_entity_manager.for_each([&](sptr<camera_entity> e, id_t const id)
			{
				e->update(dt);
			});
	}
	void scene_service::draw_all()
	{
		s_instance->m_entity_manager.for_each([](sptr<entity> e, id_t const id)
			{
				e->draw();
			});
		s_instance->m_camera_entity_manager.for_each([](sptr<camera_entity> e, id_t const id)
			{
				e->draw();
			});
	}
	void scene_service::draw_ui_all()
	{
		s_instance->m_entity_manager.for_each([](sptr<entity> e, id_t const id)
			{
				e->draw_ui();
			});
		s_instance->m_camera_entity_manager.for_each([](sptr<camera_entity> e, id_t const id)
			{
				e->draw_ui();
			});
	}


	scene_service::scene_service() :
		haul::parent<service<scene_service>>("Scene"),
		m_runtime(JS_NewRuntime()),
		m_entity_manager(m_runtime),
		m_camera_entity_manager(m_runtime)
	{}
} // namespace djinn

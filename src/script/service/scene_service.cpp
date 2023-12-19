#include "pch.h"
#include "scene_service.h"
#include "script/js.h"

namespace djinn::js::scene_service
{
	JSValue load_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		return js::create_id(ctx, ::djinn::scene_service::get_entity_manager()->load(fp));
	}
	JSValue load_camera(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		return js::create_id(ctx, ::djinn::scene_service::get_entity_manager()->load_camera(fp));
	}
	JSValue get_entity_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(false);
		return JSValue();
	}
	JSValue set_entity_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(false);
		return JSValue();
	}
	JSValue destroy_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_entity_manager()->destroy(id);
		return JS_UNDEFINED;
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
		camera_entity* const cam = ::djinn::scene_service::get_entity_manager()->get_camera(id);
		cam->move(dt, { dx, dy, dz }, mx, my);
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
	void scene_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "loadEntity", 1, js::scene_service::load_entity);
		super::register_function(ctx, "loadCamera", 1, js::scene_service::load_camera);
		super::register_function(ctx, "getEntityPos", 1, js::scene_service::get_entity_pos);
		super::register_function(ctx, "setEntityPos", 2, js::scene_service::set_entity_pos);
		super::register_function(ctx, "destroyEntity", 1, js::scene_service::destroy_entity);
		super::register_function(ctx, "moveCamera", 1, js::scene_service::move_camera);
	}
	entity_manager* scene_service::get_entity_manager()
	{
		return &s_instance->m_entity_manager;
	}


	scene_service::scene_service() :
		haul::parent<service<scene_service>>("Scene")
	{}
} // namespace djinn

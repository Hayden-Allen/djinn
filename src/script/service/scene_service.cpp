#include "pch.h"
#include "scene_service.h"
#include "script/js.h"
#include "core/constants.h"
#include "scene/camera_entity.h"

namespace djinn::js::scene_service
{
	entity* get_entity(id_t const id)
	{
		if (::djinn::scene_service::get_entity_manager()->has(id))
		{
			return ::djinn::scene_service::get_entity_manager()->get(id).get();
		}
		return ::djinn::scene_service::get_camera_entity_manager()->get(id).get();
	}



	JSValue load_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		// return js::create_id(ctx, ::djinn::scene_service::get_entity_manager()->load(fp));
		id_t const id = ::djinn::scene_service::get_entity_manager()->load(fp);
		sptr<entity> e = ::djinn::scene_service::get_entity_manager()->get(id);
		return JS_DupValue(ctx, e->get_js_value());
	}
	JSValue destroy_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		if (::djinn::scene_service::get_entity_manager()->has(id))
			::djinn::scene_service::get_entity_manager()->destroy(id);
		else
			::djinn::scene_service::get_camera_entity_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue load_camera(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		id_t const id = ::djinn::scene_service::get_camera_entity_manager()->load(fp);
		sptr<camera_entity> e = ::djinn::scene_service::get_camera_entity_manager()->get(id);
		return JS_DupValue(ctx, e->get_js_value());
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



	JSValue get_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32_array(ctx, 3, get_entity(id)->get_pos());
	}
	JSValue get_pos_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32(ctx, get_entity(id)->get_pos()[0]);
	}
	JSValue get_pos_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32(ctx, get_entity(id)->get_pos()[1]);
	}
	JSValue get_pos_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32(ctx, get_entity(id)->get_pos()[2]);
	}
	JSValue set_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		get_entity(id)->set_pos(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue set_pos_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->set_pos_x(f);
		return JS_UNDEFINED;
	}
	JSValue set_pos_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->set_pos_y(f);
		return JS_UNDEFINED;
	}
	JSValue set_pos_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->set_pos_z(f);
		return JS_UNDEFINED;
	}
	JSValue add_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		get_entity(id)->add_pos(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue add_pos_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_pos(f, 0, 0);
		return JS_UNDEFINED;
	}
	JSValue add_pos_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_pos(0, f, 0);
		return JS_UNDEFINED;
	}
	JSValue add_pos_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_pos(0, 0, f);
		return JS_UNDEFINED;
	}
	JSValue add_pos_local(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		get_entity(id)->add_pos_local(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue add_pos_local_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_pos_local(f, 0, 0);
		return JS_UNDEFINED;
	}
	JSValue add_pos_local_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_pos_local(0, f, 0);
		return JS_UNDEFINED;
	}
	JSValue add_pos_local_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_pos_local(0, 0, f);
		return JS_UNDEFINED;
	}



	JSValue get_rot(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32_array(ctx, 3, get_entity(id)->get_rot());
	}
	JSValue get_rot_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32(ctx, get_entity(id)->get_rot()[0]);
	}
	JSValue get_rot_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32(ctx, get_entity(id)->get_rot()[1]);
	}
	JSValue get_rot_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32(ctx, get_entity(id)->get_rot()[2]);
	}
	JSValue set_rot(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		get_entity(id)->set_rot(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue set_rot_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->set_rot_x(f);
		return JS_UNDEFINED;
	}
	JSValue set_rot_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->set_rot_y(f);
		return JS_UNDEFINED;
	}
	JSValue set_rot_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->set_rot_z(f);
		return JS_UNDEFINED;
	}
	JSValue add_rot(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		get_entity(id)->add_rot(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue add_rot_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_rot(f, 0, 0);
		return JS_UNDEFINED;
	}
	JSValue add_rot_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_rot(0, f, 0);
		return JS_UNDEFINED;
	}
	JSValue add_rot_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_rot(0, 0, f);
		return JS_UNDEFINED;
	}
	JSValue add_rot_local(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		get_entity(id)->add_rot_local(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue add_rot_local_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_rot_local(f, 0, 0);
		return JS_UNDEFINED;
	}
	JSValue add_rot_local_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_rot_local(0, f, 0);
		return JS_UNDEFINED;
	}
	JSValue add_rot_local_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_rot_local(0, 0, f);
		return JS_UNDEFINED;
	}



	JSValue get_scale(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32_array(ctx, 3, get_entity(id)->get_scale());
	}
	JSValue get_scale_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32(ctx, get_entity(id)->get_scale()[0]);
	}
	JSValue get_scale_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32(ctx, get_entity(id)->get_scale()[1]);
	}
	JSValue get_scale_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32(ctx, get_entity(id)->get_scale()[2]);
	}
	JSValue set_scale(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		get_entity(id)->set_scale(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue set_scale_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->set_scale_x(f);
		return JS_UNDEFINED;
	}
	JSValue set_scale_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->set_scale_y(f);
		return JS_UNDEFINED;
	}
	JSValue set_scale_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->set_scale_z(f);
		return JS_UNDEFINED;
	}
	JSValue add_scale(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		get_entity(id)->add_scale(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue add_scale_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_scale(f, 0, 0);
		return JS_UNDEFINED;
	}
	JSValue add_scale_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_scale(0, f, 0);
		return JS_UNDEFINED;
	}
	JSValue add_scale_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_scale(0, 0, f);
		return JS_UNDEFINED;
	}
	JSValue add_scale_local(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		get_entity(id)->add_scale_local(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue add_scale_local_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_scale_local(f, 0, 0);
		return JS_UNDEFINED;
	}
	JSValue add_scale_local_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_scale_local(0, f, 0);
		return JS_UNDEFINED;
	}
	JSValue add_scale_local_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		get_entity(id)->add_scale_local(0, 0, f);
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
		super::register_function(ctx, "load", 1, js::scene_service::load_entity);
		super::register_function(ctx, "destroy", 1, js::scene_service::destroy_entity);
		super::register_function(ctx, "Camera", "load", 1, js::scene_service::load_camera);
		super::register_function(ctx, "Camera", "configure", 5, js::scene_service::configure_camera);

		super::register_function(ctx, "getPos", 1, js::scene_service::get_pos);
		super::register_function(ctx, "getPosX", 1, js::scene_service::get_pos_x);
		super::register_function(ctx, "getPosY", 1, js::scene_service::get_pos_y);
		super::register_function(ctx, "getPosZ", 1, js::scene_service::get_pos_z);
		super::register_function(ctx, "setPos", 2, js::scene_service::set_pos);
		super::register_function(ctx, "setPosX", 2, js::scene_service::set_pos_x);
		super::register_function(ctx, "setPosY", 2, js::scene_service::set_pos_y);
		super::register_function(ctx, "setPosZ", 2, js::scene_service::set_pos_z);
		super::register_function(ctx, "addPos", 2, js::scene_service::add_pos);
		super::register_function(ctx, "addPosX", 2, js::scene_service::add_pos_x);
		super::register_function(ctx, "addPosY", 2, js::scene_service::add_pos_y);
		super::register_function(ctx, "addPosZ", 2, js::scene_service::add_pos_z);
		super::register_function(ctx, "addPosLocal", 2, js::scene_service::add_pos_local);
		super::register_function(ctx, "addPosLocalX", 2, js::scene_service::add_pos_local_x);
		super::register_function(ctx, "addPosLocalY", 2, js::scene_service::add_pos_local_y);
		super::register_function(ctx, "addPosLocalZ", 2, js::scene_service::add_pos_local_z);

		super::register_function(ctx, "getRot", 1, js::scene_service::get_rot);
		super::register_function(ctx, "getRotX", 1, js::scene_service::get_rot_x);
		super::register_function(ctx, "getRotY", 1, js::scene_service::get_rot_y);
		super::register_function(ctx, "getRotZ", 1, js::scene_service::get_rot_z);
		super::register_function(ctx, "setRot", 2, js::scene_service::set_rot);
		super::register_function(ctx, "setRotX", 2, js::scene_service::set_rot_x);
		super::register_function(ctx, "setRotY", 2, js::scene_service::set_rot_y);
		super::register_function(ctx, "setRotZ", 2, js::scene_service::set_rot_z);
		super::register_function(ctx, "addRot", 2, js::scene_service::add_rot);
		super::register_function(ctx, "addRotX", 2, js::scene_service::add_rot_x);
		super::register_function(ctx, "addRotY", 2, js::scene_service::add_rot_y);
		super::register_function(ctx, "addRotZ", 2, js::scene_service::add_rot_z);
		super::register_function(ctx, "addRotLocal", 2, js::scene_service::add_rot_local);
		super::register_function(ctx, "addRotLocalX", 2, js::scene_service::add_rot_local_x);
		super::register_function(ctx, "addRotLocalY", 2, js::scene_service::add_rot_local_y);
		super::register_function(ctx, "addRotLocalZ", 2, js::scene_service::add_rot_local_z);

		super::register_function(ctx, "getScale", 1, js::scene_service::get_scale);
		super::register_function(ctx, "getScaleX", 1, js::scene_service::get_scale_x);
		super::register_function(ctx, "getScaleY", 1, js::scene_service::get_scale_y);
		super::register_function(ctx, "getScaleZ", 1, js::scene_service::get_scale_z);
		super::register_function(ctx, "setScale", 2, js::scene_service::set_scale);
		super::register_function(ctx, "setScaleX", 2, js::scene_service::set_scale_x);
		super::register_function(ctx, "setScaleY", 2, js::scene_service::set_scale_y);
		super::register_function(ctx, "setScaleZ", 2, js::scene_service::set_scale_z);
		super::register_function(ctx, "addScale", 2, js::scene_service::add_scale);
		super::register_function(ctx, "addScaleX", 2, js::scene_service::add_scale_x);
		super::register_function(ctx, "addScaleY", 2, js::scene_service::add_scale_y);
		super::register_function(ctx, "addScaleZ", 2, js::scene_service::add_scale_z);
		super::register_function(ctx, "addScaleLocal", 2, js::scene_service::add_scale_local);
		super::register_function(ctx, "addScaleLocalX", 2, js::scene_service::add_scale_local_x);
		super::register_function(ctx, "addScaleLocalY", 2, js::scene_service::add_scale_local_y);
		super::register_function(ctx, "addScaleLocalZ", 2, js::scene_service::add_scale_local_z);
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

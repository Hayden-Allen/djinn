#include "pch.h"
#include "scene_service.h"
#include "script/js.h"
#include "core/constants.h"
#include "scene/camera_entity.h"
#include "asset_service.h"
#include "scene/animated_mesh_instance.h"

namespace djinn::js::scene_service
{
	static entity* get_entity(id_t const id)
	{
		if (::djinn::scene_service::get_entity_manager()->has(id))
		{
			return ::djinn::scene_service::get_entity_manager()->get(id).get();
		}
		return ::djinn::scene_service::get_camera_entity_manager()->get(id).get();
	}
	template<typename FN>
	JSValue get_value(JSContext* const ctx, s32 const argc, JSValueConst* const argv, FN const& fn, u64 const index)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32(ctx, (::djinn::scene_service::get_scene_object(id).get()->*fn)()[index]);
	}
	template<typename FN>
	JSValue get_values(JSContext* const ctx, s32 const argc, JSValueConst* const argv, FN const& fn)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		return js::create_f32_array(ctx, 3, (::djinn::scene_service::get_scene_object(id).get()->*fn)());
	}
	template<typename FN>
	JSValue set_value(JSContext* const ctx, s32 const argc, JSValueConst* const argv, FN const& fn)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		(::djinn::scene_service::get_scene_object(id).get()->*fn)(f);
		return JS_UNDEFINED;
	}
	template<typename FN>
	JSValue set_values(JSContext* const ctx, s32 const argc, JSValueConst* const argv, FN const& fn)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		(::djinn::scene_service::get_scene_object(id).get()->*fn)(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	template<typename FN>
	JSValue add_value(JSContext* const ctx, s32 const argc, JSValueConst* const argv, FN const& fn, f32 const x, f32 const y, f32 const z)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		(::djinn::scene_service::get_scene_object(id).get()->*fn)(x * f, y * f, z * f);
		return JS_UNDEFINED;
	}



	JSValue create_light(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_id(ctx, ::djinn::scene_service::get_light_manager()->create());
	}
	JSValue set_light_color_ambient(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& rgba = js::extract_f32_array(ctx, argv[1]);
		ASSERT(rgba.size() == 4);
		mgl::light& l = ::djinn::scene_service::get_light_manager()->get(id)->get_raw();
		for (u64 i = 0; i < rgba.size(); i++)
			l.ca[i] = rgba[i];
		return JS_UNDEFINED;
	}
	JSValue set_light_color_diffuse(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& rgba = js::extract_f32_array(ctx, argv[1]);
		ASSERT(rgba.size() == 4);
		mgl::light& l = ::djinn::scene_service::get_light_manager()->get(id)->get_raw();
		for (u64 i = 0; i < rgba.size(); i++)
			l.cd[i] = rgba[i];
		return JS_UNDEFINED;
	}
	JSValue set_light_color_specular(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& rgba = js::extract_f32_array(ctx, argv[1]);
		ASSERT(rgba.size() == 4);
		mgl::light& l = ::djinn::scene_service::get_light_manager()->get(id)->get_raw();
		for (u64 i = 0; i < rgba.size(); i++)
			l.cs[i] = rgba[i];
		return JS_UNDEFINED;
	}
	JSValue destroy_light(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_light_manager()->destroy(id);
		return JS_UNDEFINED;
	}



	JSValue load_xport(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		std::string const& afp = u::to_absolute(c::base_dir::xport, fp);
		mgl::input_file in(afp);

		std::vector<id_t> const& phorms = ::djinn::scene_service::get_phorm_manager()->load_all(&in);
		std::vector<id_t> const& lights = ::djinn::scene_service::get_light_manager()->load_all(&in);
		// return js::create_id_array(ctx, (s64)ids.size(), ids.data());
		JSValue map = JS_NewObject(ctx);
		JS_SetPropertyStr(ctx, map, "phorms", js::create_id_array(ctx, (s64)phorms.size(), phorms.data()));
		JS_SetPropertyStr(ctx, map, "lights", js::create_id_array(ctx, (s64)lights.size(), lights.data()));
		return map;
	}
	JSValue set_phorm_shaders(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id_phorm = js::extract_id(ctx, argv[0]);
		id_t const id_shaders = js::extract_id(ctx, argv[1]);

		sptr<phorm> phorm = ::djinn::scene_service::get_phorm_manager()->get(id_phorm);
		sptr<shaders> shaders = ::djinn::asset_service::get_shader_manager()->get(id_shaders);
		phorm->set_shaders(shaders);
		return JS_UNDEFINED;
	}
	JSValue set_phorm_visible(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id_phorm = js::extract_id(ctx, argv[0]);
		bool const visible = js::extract_bool(ctx, argv[1]);
		::djinn::scene_service::get_phorm_manager()->get(id_phorm)->set_visible(visible);
		return JS_UNDEFINED;
	}
	JSValue destroy_phorm(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_phorm_manager()->destroy(id);
		return JS_UNDEFINED;
	}



	JSValue request_imgui(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		get_entity(id)->request_imgui();
		return JS_UNDEFINED;
	}
	JSValue copy_transform(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const phys_id = js::extract_id(ctx, argv[0]);
		id_t const target_id = js::extract_id(ctx, argv[1]);
		sptr<physics_object> po = ::djinn::scene_service::get_physics_object_manager()->get(phys_id);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(target_id);
		so->copy_transform(po);
		return JS_UNDEFINED;
	}



	JSValue load_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
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



	JSValue create_mesh_instance(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const mesh_id = js::extract_id(ctx, argv[0]);
		sptr<mesh> mesh = ::djinn::asset_service::get_mesh(mesh_id);
		id_t const shader_id = js::extract_id(ctx, argv[1]);
		sptr<shaders> const& shaders = ::djinn::asset_service::get_shader_manager()->get(shader_id);
		id_t const instance_id = ::djinn::scene_service::get_mesh_instance_manager()->create(mesh, shaders);
		return js::create_id(ctx, instance_id);
	}
	JSValue set_mesh_instance_uniforms(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const instance_id = js::extract_id(ctx, argv[0]);
		std::unordered_map<std::string, JSValue> const& map = js::extract_map(ctx, argv[1]);
		sptr<mesh_instance> instance = ::djinn::scene_service::get_mesh_instance_manager()->get(instance_id);
		for (auto const& pair : map)
		{
			ASSERT(JS_IsArray(ctx, pair.second));
			ASSERT(js::extract_array_length(ctx, pair.second) == 2);
			JSValue const& js_data = JS_GetPropertyUint32(ctx, pair.second, 0);
			JSValue const& js_index = JS_GetPropertyUint32(ctx, pair.second, 1);
			std::vector<f32> const& data = js::extract_f32_array(ctx, js_data);
			u32 const index = js::extract_u32(ctx, js_index);
			JS_FreeValue(ctx, js_data);
			JS_FreeValue(ctx, js_index);
			instance->set_uniform(pair.first, data, index);
		}
		return JS_UNDEFINED;
	}
	JSValue set_mesh_instance_action(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2 || argc == 3);
		id_t const instance_id = js::extract_id(ctx, argv[0]);
		std::string const& action = js::extract_string(ctx, argv[1]);
		f32 speed = 1.f;
		if (argc == 3)
			speed = js::extract_f32(ctx, argv[2]);

		sptr<mesh_instance> instance = ::djinn::scene_service::get_mesh_instance_manager()->get(instance_id);
		ASSERT(instance->is_animated());
		sptr<animated_mesh_instance> ami = instance;

		ami->set_action(action, speed);
		return JS_UNDEFINED;
	}
	JSValue set_mesh_instance_visible(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		bool const visible = js::extract_bool(ctx, argv[1]);
		::djinn::scene_service::get_mesh_instance_manager()->get(id)->set_visible(visible);
		return JS_UNDEFINED;
	}
	JSValue destroy_mesh_instance(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_mesh_instance_manager()->destroy(id);
		return JS_UNDEFINED;
	}



	JSValue create_physics_object(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 3);
		std::vector<f32> const& dimsarr = js::extract_f32_array(ctx, argv[0]);
		std::vector<f32> const& originarr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(dimsarr.size() == 3 && originarr.size() == 3);
		f32 const mass = js::extract_f32(ctx, argv[2]);
		ASSERT(mass >= 0.f);

		btVector3 const dims(dimsarr[0], dimsarr[1], dimsarr[2]);
		btVector3 const origin(originarr[0], originarr[1], originarr[2]);

		return js::create_id(ctx, ::djinn::scene_service::get_physics_object_manager()->create(dims, origin, mass));
	}
	JSValue set_friction(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_friction(f);
		return JS_UNDEFINED;
	}
	JSValue set_linear_velocity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_linear_velocity(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue set_angular_velocity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_angular_velocity(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue enable_collision(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_collision_enabled(true);
		return JS_UNDEFINED;
	}
	JSValue disable_collision(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_collision_enabled(false);
		return JS_UNDEFINED;
	}
	JSValue destroy_physics_object(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_physics_object_manager()->destroy(id);
		return JS_UNDEFINED;
	}



	JSValue get_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_values(ctx, argc, argv, &scene_object_base::get_pos);
	}
	JSValue get_pos_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object_base::get_pos, 0);
	}
	JSValue get_pos_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object_base::get_pos, 1);
	}
	JSValue get_pos_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object_base::get_pos, 2);
	}
	JSValue set_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values(ctx, argc, argv, &scene_object_base::set_pos);
	}
	JSValue set_pos_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object_base::set_pos_x);
	}
	JSValue set_pos_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object_base::set_pos_y);
	}
	JSValue set_pos_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object_base::set_pos_z);
	}
	JSValue add_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values(ctx, argc, argv, &scene_object_base::add_pos);
	}
	JSValue add_pos_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_pos, 1, 0, 0);
	}
	JSValue add_pos_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_pos, 0, 1, 0);
	}
	JSValue add_pos_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_pos, 0, 0, 1);
	}
	JSValue add_pos_local(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values(ctx, argc, argv, &scene_object_base::add_pos_local);
	}
	JSValue add_pos_local_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_pos_local, 1, 0, 0);
	}
	JSValue add_pos_local_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_pos_local, 0, 1, 0);
	}
	JSValue add_pos_local_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_pos_local, 0, 0, 1);
	}



	JSValue get_rot(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_values(ctx, argc, argv, &scene_object_base::get_rot);
	}
	JSValue get_rot_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object_base::get_rot, 0);
	}
	JSValue get_rot_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object_base::get_rot, 1);
	}
	JSValue get_rot_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object_base::get_rot, 2);
	}
	JSValue set_rot(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values(ctx, argc, argv, &scene_object_base::set_rot);
	}
	JSValue set_rot_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object_base::set_rot_x);
	}
	JSValue set_rot_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object_base::set_rot_y);
	}
	JSValue set_rot_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object_base::set_rot_z);
	}
	JSValue add_rot(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values(ctx, argc, argv, &scene_object_base::add_rot);
	}
	JSValue add_rot_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_rot, 1, 0, 0);
	}
	JSValue add_rot_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_rot, 0, 1, 0);
	}
	JSValue add_rot_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_rot, 0, 0, 1);
	}
	JSValue add_rot_local(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values(ctx, argc, argv, &scene_object_base::add_rot_local);
	}
	JSValue add_rot_local_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_rot_local, 1, 0, 0);
	}
	JSValue add_rot_local_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_rot_local, 0, 1, 0);
	}
	JSValue add_rot_local_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_rot_local, 0, 0, 1);
	}



	JSValue get_scale(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_values(ctx, argc, argv, &scene_object_base::get_scale);
	}
	JSValue get_scale_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object_base::get_scale, 0);
	}
	JSValue get_scale_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object_base::get_scale, 1);
	}
	JSValue get_scale_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object_base::get_scale, 2);
	}
	JSValue set_scale(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values(ctx, argc, argv, &scene_object_base::set_scale);
	}
	JSValue set_scale_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object_base::set_scale_x);
	}
	JSValue set_scale_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object_base::set_scale_y);
	}
	JSValue set_scale_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object_base::set_scale_z);
	}
	JSValue add_scale(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values(ctx, argc, argv, &scene_object_base::add_scale);
	}
	JSValue add_scale_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_scale, 1, 0, 0);
	}
	JSValue add_scale_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_scale, 0, 1, 0);
	}
	JSValue add_scale_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_scale, 0, 0, 1);
	}
	JSValue add_scale_local(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values(ctx, argc, argv, &scene_object_base::add_scale_local);
	}
	JSValue add_scale_local_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_scale_local, 1, 0, 0);
	}
	JSValue add_scale_local_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_scale_local, 0, 1, 0);
	}
	JSValue add_scale_local_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return add_value(ctx, argc, argv, &scene_object_base::add_scale_local, 0, 0, 1);
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
	void scene_service::free_all()
	{
		scene_service::get_camera_entity_manager()->free_all();
		scene_service::get_entity_manager()->free_all();
	}
	void scene_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "Light", "create", 0, js::scene_service::create_light);
		super::register_function(ctx, "Light", "setAmbient", 2, js::scene_service::set_light_color_ambient);
		super::register_function(ctx, "Light", "setDiffuse", 2, js::scene_service::set_light_color_diffuse);
		super::register_function(ctx, "Light", "setSpecular", 2, js::scene_service::set_light_color_specular);
		super::register_function(ctx, "Light", "destroy", 1, js::scene_service::destroy_light);

		super::register_function(ctx, "Xport", "load", 1, js::scene_service::load_xport);

		super::register_function(ctx, "Phorm", "setShaders", 2, js::scene_service::set_phorm_shaders);
		super::register_function(ctx, "Phorm", "setVisible", 2, js::scene_service::set_phorm_visible);
		super::register_function(ctx, "Phorm", "destroy", 1, js::scene_service::destroy_phorm);

		super::register_function(ctx, "MeshInstance", "create", 2, js::scene_service::create_mesh_instance);
		super::register_function(ctx, "MeshInstance", "setUniforms", 2, js::scene_service::set_mesh_instance_uniforms);
		super::register_function(ctx, "MeshInstance", "setAction", 3, js::scene_service::set_mesh_instance_action);
		super::register_function(ctx, "MeshInstance", "setVisible", 2, js::scene_service::set_mesh_instance_visible);
		super::register_function(ctx, "MeshInstance", "destroy", 1, js::scene_service::destroy_mesh_instance);

		super::register_function(ctx, "Entity", "requestImGui", 1, js::scene_service::request_imgui);
		super::register_function(ctx, "Entity", "load", 1, js::scene_service::load_entity);
		super::register_function(ctx, "Entity", "destroy", 1, js::scene_service::destroy_entity);

		super::register_function(ctx, "Camera", "load", 1, js::scene_service::load_camera);
		super::register_function(ctx, "Camera", "configure", 5, js::scene_service::configure_camera);

		super::register_function(ctx, "Physics", "create", 3, js::scene_service::create_physics_object);
		super::register_function(ctx, "Physics", "setFriction", 2, js::scene_service::set_friction);
		super::register_function(ctx, "Physics", "setLinearVelocity", 2, js::scene_service::set_linear_velocity);
		super::register_function(ctx, "Physics", "setAngularVelocity", 2, js::scene_service::set_angular_velocity);
		super::register_function(ctx, "Physics", "enableCollision", 1, js::scene_service::enable_collision);
		super::register_function(ctx, "Physics", "disableCollision", 1, js::scene_service::disable_collision);
		super::register_function(ctx, "Physics", "destroy", 1, js::scene_service::destroy_physics_object);

		super::register_function(ctx, "copyTransform", 2, js::scene_service::copy_transform);
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
	mesh_instance_manager* scene_service::get_mesh_instance_manager()
	{
		return &s_instance->m_mesh_instance_manager;
	}
	physics_object_manager* scene_service::get_physics_object_manager()
	{
		return &s_instance->m_physics_object_manager;
	}
	phorm_manager* scene_service::get_phorm_manager()
	{
		return &s_instance->m_phorm_manager;
	}
	light_manager* scene_service::get_light_manager()
	{
		return &s_instance->m_light_manager;
	}
	JSRuntime* scene_service::get_runtime()
	{
		return s_instance->m_runtime;
	}
	void scene_service::update(f32 const dt, f32 const time)
	{
		s_instance->m_entity_manager.for_each([&](sptr<entity> e, id_t const id)
			{
				e->update(dt, time);
			});
		s_instance->m_camera_entity_manager.for_each([&](sptr<camera_entity> e, id_t const id)
			{
				e->update(dt, time);
			});
		s_instance->m_physics_object_manager.update(dt);
		s_instance->m_light_manager.update();
	}
	void scene_service::draw()
	{
		s_instance->m_entity_manager.for_each([](sptr<entity> e, id_t const id)
			{
				e->draw();
			});
		s_instance->m_camera_entity_manager.for_each([](sptr<camera_entity> e, id_t const id)
			{
				e->draw();
			});
		s_instance->m_phorm_manager.for_each([](sptr<phorm> p, id_t const id)
			{
				p->draw(render_service::get_context());
			});
	}
	void scene_service::draw_ui()
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
	void scene_service::draw_imgui()
	{
		s_instance->m_entity_manager.for_each([](sptr<entity> e, id_t const id)
			{
				e->draw_imgui();
			});
		s_instance->m_camera_entity_manager.for_each([](sptr<camera_entity> e, id_t const id)
			{
				e->draw_imgui();
			});
	}
	sptr<scene_object> scene_service::get_scene_object(id_t const id)
	{
		if (get_entity_manager()->has(id))
			return get_entity_manager()->get(id);
		else if (get_camera_entity_manager()->has(id))
			return get_camera_entity_manager()->get(id);
		else if (get_physics_object_manager()->has(id))
			return get_physics_object_manager()->get(id);
		else if (get_phorm_manager()->has(id))
			return get_phorm_manager()->get(id);
		else if (get_light_manager()->has(id))
			return get_light_manager()->get(id);
		return get_mesh_instance_manager()->get(id);
	}



	scene_service::scene_service() :
		haul::parent<service<scene_service>>("Scene"),
		m_runtime(JS_NewRuntime()),
		m_entity_manager(m_runtime),
		m_camera_entity_manager(m_runtime)
	{}
} // namespace djinn

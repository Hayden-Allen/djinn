#include "pch.h"
#include "scene_service.h"
#include "script/js.h"
#include "core/constants.h"
#include "scene/entity/camera_entity.h"
#include "asset_service.h"
#include "scene/mesh/animated_mesh_instance.h"
#include "asset/xport/material.h"

namespace djinn::js::scene_service
{
	//
	//	HELPER
	//
	static entity* get_entity(id_t const id)
	{
		if (::djinn::scene_service::get_entity_manager()->has(id))
			return ::djinn::scene_service::get_entity_manager()->get(id).get();
		return ::djinn::scene_service::get_camera_entity_manager()->get(id).get();
	}
	template<typename FN>
	JSValue get_values(JSContext* const ctx, s32 const argc, JSValueConst* const argv, FN const& fn)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(id);
		return js::create_f32_array(ctx, 3, (so.get()->*fn)().e);
	}
	template<typename FN>
	JSValue get_value(JSContext* const ctx, s32 const argc, JSValueConst* const argv, FN const& fn, u32 const index)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(id);
		return js::create_f32(ctx, (so.get()->*fn)()[index]);
	}
	template<typename ARG, typename FN>
	JSValue set_values(JSContext* const ctx, s32 const argc, JSValueConst* const argv, FN const& fn)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(arr.size() == 3);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(id);
		(so.get()->*fn)(ARG(arr[0], arr[1], arr[2]));
		return JS_UNDEFINED;
	}
	template<typename FN>
	JSValue set_value(JSContext* const ctx, s32 const argc, JSValueConst* const argv, FN const& fn)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(id);
		(so.get()->*fn)(f);
		return JS_UNDEFINED;
	}

	//
	// TAGGED
	//
	JSValue add_tag(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::string const& tag = js::extract_string(ctx, argv[1]);
		::djinn::scene_service::get_tagged(id)->add_tag(tag);
		return JS_UNDEFINED;
	}
	JSValue has_tag(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::string const& tag = js::extract_string(ctx, argv[1]);
		return js::create_bool(ctx, ::djinn::scene_service::get_tagged(id)->has_tag(tag));
	}
	JSValue get_tags(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::unordered_set<std::string> const& tags = ::djinn::scene_service::get_tagged(id)->get_tags();
		return js::create_string_array(ctx, tags.begin(), tags.end());
	}
	JSValue remove_tag(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::string const& tag = js::extract_string(ctx, argv[1]);
		::djinn::scene_service::get_tagged(id)->remove_tag(tag);
		return JS_UNDEFINED;
	}

	//
	//	WAYPOINT
	//
	JSValue create_waypoint(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_id(ctx, ::djinn::scene_service::get_waypoint_manager()->create());
	}
	JSValue destroy_waypoint(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_waypoint_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue destroy_all_waypoint(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::vector<id_t> const& ids = js::extract_id_array(ctx, argv[0]);
		for (id_t const id : ids)
			::djinn::scene_service::get_waypoint_manager()->destroy(id);
		return JS_UNDEFINED;
	}

	//
	//	LIGHT
	//
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
	JSValue destroy_all_light(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::vector<id_t> const& ids = js::extract_id_array(ctx, argv[0]);
		for (id_t const id : ids)
			::djinn::scene_service::get_light_manager()->destroy(id);
		return JS_UNDEFINED;
	}

	//
	//	PHORM
	//
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
	JSValue set_phorm_alpha_shaders(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id_phorm = js::extract_id(ctx, argv[0]);
		id_t const id_shaders = js::extract_id(ctx, argv[1]);

		sptr<phorm> phorm = ::djinn::scene_service::get_phorm_manager()->get(id_phorm);
		sptr<shaders> shaders = ::djinn::asset_service::get_shader_manager()->get(id_shaders);
		phorm->set_alpha_shaders(shaders);
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
	JSValue destroy_all_phorm(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::vector<id_t> const& ids = js::extract_id_array(ctx, argv[0]);
		for (id_t const id : ids)
			::djinn::scene_service::get_phorm_manager()->destroy(id);
		return JS_UNDEFINED;
	}

	//
	// XPORT
	//
	JSValue load_xport(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		std::string const& afp = u::to_absolute(c::base_dir::xport, fp);
		mgl::input_file in(afp);

		auto const& [tex_ids, tex] = ::djinn::asset_service::get_texture_manager()->load_xport(&in);
		id_t const skybox_id = ::djinn::asset_service::get_cubemap_manager()->load_xport(&in);
		// materials are not accesible to the scripts, just used internally for xports
		std::unordered_map<u32, sptr<material>> const& materials = ::djinn::asset_service::get_material_manager()->load_xport(&in, tex);
		std::vector<id_t> const& phorms = ::djinn::scene_service::get_phorm_manager()->load_xport(&in, materials);
		printf("PHORMZ:\n");
		for (id_t const id : phorms)
			printf("\t%u\n", id);
		std::vector<id_t> const& lights = ::djinn::scene_service::get_light_manager()->load_xport(&in);
		std::vector<id_t> const& waypoints = ::djinn::scene_service::get_waypoint_manager()->load_xport(&in);

		JSValue map = JS_NewObject(ctx);
		JS_SetPropertyStr(ctx, map, "textures", js::create_id_array(ctx, (s64)tex_ids.size(), tex_ids.data()));
		JS_SetPropertyStr(ctx, map, "skybox", js::create_id(ctx, skybox_id));
		JS_SetPropertyStr(ctx, map, "phorms", js::create_id_array(ctx, (s64)phorms.size(), phorms.data()));
		JS_SetPropertyStr(ctx, map, "lights", js::create_id_array(ctx, (s64)lights.size(), lights.data()));
		JS_SetPropertyStr(ctx, map, "waypoints", js::create_id_array(ctx, (s64)waypoints.size(), waypoints.data()));
		return map;
	}
	JSValue get_xport_name(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		xport const* const x = ::djinn::scene_service::get_xport(id);
		return js::create_string(ctx, x->get_name());
	}

	//
	//	ENTITY / CAMERA
	//
	JSValue load_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc >= 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		id_t const id = ::djinn::scene_service::get_entity_manager()->load(fp, argc - 1, &argv[1]);
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
	JSValue destroy_all_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::vector<id_t> const& ids = js::extract_id_array(ctx, argv[0]);
		for (id_t const id : ids)
		{
			if (::djinn::scene_service::get_entity_manager()->has(id))
				::djinn::scene_service::get_entity_manager()->destroy(id);
			else
				::djinn::scene_service::get_camera_entity_manager()->destroy(id);
		}
		return JS_UNDEFINED;
	}
	JSValue load_camera(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		id_t const id = ::djinn::scene_service::get_camera_entity_manager()->load(fp, argc - 1, &argv[1]);
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
	JSValue request_imgui(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		get_entity(id)->request_imgui();
		return JS_UNDEFINED;
	}

	//
	//	MESH INSTANCE
	//
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
	JSValue set_mesh_instance_visible(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		bool const visible = js::extract_bool(ctx, argv[1]);
		::djinn::scene_service::get_mesh_instance_manager()->get(id)->set_visible(visible);
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
	JSValue get_mesh_instance_bone_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::string const& name = js::extract_string(ctx, argv[1]);
		sptr<mesh_instance> instance = ::djinn::scene_service::get_mesh_instance_manager()->get(id);
		ASSERT(instance->is_animated());
		sptr<animated_mesh_instance> ami = instance;
		// point<space::WORLD> const& pos = ami->get_bone_pos(name);
		tmat<space::OBJECT, space::WORLD> const& mat = ami->get_bone_transform(name);
		return js::create_f32_array(ctx, 3, mat.t);
	}
	JSValue get_mesh_instance_bone_basis_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::string const& name = js::extract_string(ctx, argv[1]);
		sptr<mesh_instance> instance = ::djinn::scene_service::get_mesh_instance_manager()->get(id);
		ASSERT(instance->is_animated());
		sptr<animated_mesh_instance> ami = instance;
		// point<space::WORLD> const& pos = ami->get_bone_pos(name);
		tmat<space::OBJECT, space::WORLD> const& mat = ami->get_bone_transform(name);
		return js::create_f32_array(ctx, 3, mat.i);
	}
	JSValue get_mesh_instance_bone_basis_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::string const& name = js::extract_string(ctx, argv[1]);
		sptr<mesh_instance> instance = ::djinn::scene_service::get_mesh_instance_manager()->get(id);
		ASSERT(instance->is_animated());
		sptr<animated_mesh_instance> ami = instance;
		// point<space::WORLD> const& pos = ami->get_bone_pos(name);
		tmat<space::OBJECT, space::WORLD> const& mat = ami->get_bone_transform(name);
		return js::create_f32_array(ctx, 3, mat.j);
	}
	JSValue get_mesh_instance_bone_basis_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::string const& name = js::extract_string(ctx, argv[1]);
		sptr<mesh_instance> instance = ::djinn::scene_service::get_mesh_instance_manager()->get(id);
		ASSERT(instance->is_animated());
		sptr<animated_mesh_instance> ami = instance;
		// point<space::WORLD> const& pos = ami->get_bone_pos(name);
		tmat<space::OBJECT, space::WORLD> const& mat = ami->get_bone_transform(name);
		return js::create_f32_array(ctx, 3, mat.k);
	}
	JSValue destroy_mesh_instance(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_mesh_instance_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue destroy_all_mesh_instance(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::vector<id_t> const& ids = js::extract_id_array(ctx, argv[0]);
		for (id_t const id : ids)
			::djinn::scene_service::get_mesh_instance_manager()->destroy(id);
		return JS_UNDEFINED;
	}

	//
	//	PHYSICS
	//
	JSValue bind_physics_object(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id_phys = js::extract_id(ctx, argv[0]);
		id_t const id_bound = js::extract_id(ctx, argv[1]);
		sptr<physics_object> phys = ::djinn::scene_service::get_physics_object_manager()->get(id_phys);
		if (::djinn::scene_service::get_entity_manager()->has(id_bound))
		{
			phys->bind(::djinn::scene_service::get_entity_manager()->get(id_bound).get());
		}
		else
		{
			phys->bind(::djinn::scene_service::get_phorm_manager()->get(id_bound).get());
		}
		return JS_UNDEFINED;
	}
	JSValue create_physics_box(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 3);
		f32 const mass = js::extract_f32(ctx, argv[0]);
		ASSERT(mass >= 0.f);
		std::vector<f32> const& originarr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(originarr.size() == 3);
		std::vector<f32> const& dimsarr = js::extract_f32_array(ctx, argv[2]);

		btVector3 const dims(dimsarr[0], dimsarr[1], dimsarr[2]);
		btVector3 const origin(originarr[0], originarr[1], originarr[2]);

		return js::create_id(ctx, ::djinn::scene_service::get_physics_object_manager()->create_box(dims, origin, mass));
	}
	JSValue create_physics_cylinder(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 3);
		f32 const mass = js::extract_f32(ctx, argv[0]);
		ASSERT(mass >= 0.f);
		std::vector<f32> const& originarr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(originarr.size() == 3);
		std::vector<f32> const& dimsarr = js::extract_f32_array(ctx, argv[2]);
		ASSERT(dimsarr.size() == 3);

		btVector3 const dims(dimsarr[0], dimsarr[1], dimsarr[2]);
		btVector3 const origin(originarr[0], originarr[1], originarr[2]);

		return js::create_id(ctx, ::djinn::scene_service::get_physics_object_manager()->create_cylinder(dims, origin, mass));
	}
	JSValue create_physics_sphere(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 3);
		f32 const mass = js::extract_f32(ctx, argv[0]);
		ASSERT(mass >= 0.f);
		std::vector<f32> const& originarr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(originarr.size() == 3);
		f32 const radius = js::extract_f32(ctx, argv[2]);
		ASSERT(radius > 0.f);

		btVector3 const origin(originarr[0], originarr[1], originarr[2]);

		return js::create_id(ctx, ::djinn::scene_service::get_physics_object_manager()->create_sphere(radius, origin, mass));
	}
	JSValue create_physics_capsule_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 4);
		f32 const mass = js::extract_f32(ctx, argv[0]);
		ASSERT(mass >= 0.f);
		std::vector<f32> const& originarr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(originarr.size() == 3);
		f32 const radius = js::extract_f32(ctx, argv[2]);
		f32 const height = js::extract_f32(ctx, argv[3]);
		ASSERT(radius > 0.f && height > 0.f);

		btVector3 const origin(originarr[0], originarr[1], originarr[2]);

		return js::create_id(ctx, ::djinn::scene_service::get_physics_object_manager()->create_capsule_x(radius, height, origin, mass));
	}
	JSValue create_physics_capsule_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 4);
		f32 const mass = js::extract_f32(ctx, argv[0]);
		ASSERT(mass >= 0.f);
		std::vector<f32> const& originarr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(originarr.size() == 3);
		f32 const radius = js::extract_f32(ctx, argv[2]);
		f32 const height = js::extract_f32(ctx, argv[3]);
		ASSERT(radius > 0.f && height > 0.f);

		btVector3 const origin(originarr[0], originarr[1], originarr[2]);

		return js::create_id(ctx, ::djinn::scene_service::get_physics_object_manager()->create_capsule_y(radius, height, origin, mass));
	}
	JSValue create_physics_capsule_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 4);
		f32 const mass = js::extract_f32(ctx, argv[0]);
		ASSERT(mass >= 0.f);
		std::vector<f32> const& originarr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(originarr.size() == 3);
		f32 const radius = js::extract_f32(ctx, argv[2]);
		f32 const height = js::extract_f32(ctx, argv[3]);

		btVector3 const origin(originarr[0], originarr[1], originarr[2]);

		return js::create_id(ctx, ::djinn::scene_service::get_physics_object_manager()->create_capsule_z(radius, height, origin, mass));
	}
	JSValue create_physics_bvh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		sptr<phorm> const& phorm = ::djinn::scene_service::get_phorm_manager()->get(id);
		return js::create_id(ctx, ::djinn::scene_service::get_physics_object_manager()->create_bvh(phorm));
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
	JSValue get_velocity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		vec<space::OBJECT> const& vel = so->get_velocity();
		return js::create_f32_array(ctx, 3, vel.e);
	}
	JSValue get_speed(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		vec<space::OBJECT> const& vel = so->get_velocity();
		return js::create_f32(ctx, vel.length());
	}
	JSValue set_velocity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_velocity(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue set_velocity_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const v = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_velocity_x(v);
		return JS_UNDEFINED;
	}
	JSValue set_velocity_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const v = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_velocity_y(v);
		return JS_UNDEFINED;
	}
	JSValue set_velocity_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const v = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_velocity_z(v);
		return JS_UNDEFINED;
	}
	JSValue get_velocity_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		vec<space::WORLD> const& vel = so->get_velocity_world();
		return js::create_f32_array(ctx, 3, vel.e);
	}
	JSValue get_speed_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		vec<space::WORLD> const& vel = so->get_velocity_world();
		return js::create_f32(ctx, vel.length());
	}
	JSValue set_velocity_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_velocity_world(arr[0], arr[1], arr[2]);
		return JS_UNDEFINED;
	}
	JSValue set_velocity_x_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const v = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_velocity_x_world(v);
		return JS_UNDEFINED;
	}
	JSValue set_velocity_y_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const v = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_velocity_y_world(v);
		return JS_UNDEFINED;
	}
	JSValue set_velocity_z_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const v = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_velocity_z_world(v);
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
	JSValue set_angular_factor(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_angular_factor(arr[0], arr[1], arr[2]);
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
	JSValue apply_impulse(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& arr = js::extract_f32_array(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->apply_impulse(vec<space::OBJECT>(arr[0], arr[1], arr[2]));
		return JS_UNDEFINED;
	}
	JSValue set_damping(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const d = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_damping(d);
		return JS_UNDEFINED;
	}
	JSValue set_angular_damping(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const d = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_angular_damping(d);
		return JS_UNDEFINED;
	}
	JSValue set_max_speed_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const s = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_max_speed(0, s);
		return JS_UNDEFINED;
	}
	JSValue set_max_speed_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const s = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_max_speed(1, s);
		return JS_UNDEFINED;
	}
	JSValue set_max_speed_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const s = js::extract_f32(ctx, argv[1]);

		sptr<physics_object> so = ::djinn::scene_service::get_physics_object_manager()->get(id);
		so->set_max_speed(2, s);
		return JS_UNDEFINED;
	}
	JSValue cast_ray(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2 || argc == 3);
		std::vector<f32> const& pos = js::extract_f32_array(ctx, argv[0]);
		std::vector<f32> const& dir = js::extract_f32_array(ctx, argv[1]);
		f32 length = c::physics::default_raycast_length;
		if (argc == 3)
		{
			length = js::extract_f32(ctx, argv[2]);
			ASSERT(length > 0.f);
		}

		point<space::WORLD> const wpos(pos[0], pos[1], pos[2]);
		direction<space::WORLD> const wdir(dir[0], dir[1], dir[2]);
		std::vector<raycast_result> const& results = ::djinn::scene_service::get_physics_object_manager()->cast_ray(wpos, wdir, length);
		JSValue ret = JS_NewArray(ctx);
		for (u64 i = 0; i < results.size(); i++)
		{
			JSValue entry = JS_NewArray(ctx);
			JSValue point = js::create_f32_array(ctx, 3, results[i].pos.e);
			JSValue normal = js::create_f32_array(ctx, 3, results[i].normal.e);
			JS_SetPropertyInt64(ctx, entry, 0, point);
			JS_SetPropertyInt64(ctx, entry, 1, normal);
			JS_SetPropertyInt64(ctx, ret, (s64)i, entry);
		}
		return ret;
	}
	JSValue get_normal_tangent(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 3);
		std::vector<f32> const& norm = js::extract_f32_array(ctx, argv[0]);
		std::vector<f32> const& dir = js::extract_f32_array(ctx, argv[1]);
		id_t const id = js::extract_id(ctx, argv[2]);

		sptr<physics_object> po = ::djinn::scene_service::get_physics_object_manager()->get(id);
		direction<space::OBJECT> const odir(dir[0], dir[1], dir[2]);
		tmat<space::OBJECT, space::WORLD> const& mat = po->get_world_transform();
		direction<space::WORLD> const wdir = mat * odir;

		direction<space::WORLD> const wnorm(norm[0], norm[1], norm[2]);
		direction<space::WORLD> const wside = wnorm.cross_copy(wdir);
		direction<space::WORLD> const wtan = wside.cross_copy(wnorm);

		direction<space::OBJECT> const otan = mat.invert_copy() * wtan;
		return js::create_f32_array(ctx, 3, otan.e);
	}
	JSValue set_gravity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& force = js::extract_f32_array(ctx, argv[1]);
		ASSERT(force.size() == 3);
		::djinn::scene_service::get_physics_object_manager()->get(id)->set_gravity(vec<space::WORLD>(force[0], force[1], force[2]));
		return JS_UNDEFINED;
	}
	JSValue set_kinematic(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		bool const is_kinematic = js::extract_bool(ctx, argv[1]);
		::djinn::scene_service::get_physics_object_manager()->get(id)->set_kinematic(is_kinematic);
		return JS_UNDEFINED;
	}
	JSValue set_ghost(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		bool const is_ghost = js::extract_bool(ctx, argv[1]);
		::djinn::scene_service::get_physics_object_manager()->get(id)->set_ghost(is_ghost);
		return JS_UNDEFINED;
	}
	JSValue collide_and_slide(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 3);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& v = js::extract_f32_array(ctx, argv[1]);
		f32 const dt = js::extract_f32(ctx, argv[2]);
		ASSERT(v.size() == 3);
		::djinn::scene_service::get_physics_object_manager()->get(id)->collide_and_slide(vec<space::OBJECT>(v[0], v[1], v[2]), dt);
		return JS_UNDEFINED;
	}
	JSValue aabb_intersects(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id0 = js::extract_id(ctx, argv[0]);
		id_t const id1 = js::extract_id(ctx, argv[1]);
		sptr<physics_object> obj0 = ::djinn::scene_service::get_physics_object_manager()->get(id0);
		sptr<physics_object> obj1 = ::djinn::scene_service::get_physics_object_manager()->get(id1);
		return js::create_bool(ctx, obj0->aabb_intersects(obj1.get()));
	}
	JSValue destroy_physics_object(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_physics_object_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue destroy_all_physics_object(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::vector<id_t> const& ids = js::extract_id_array(ctx, argv[0]);
		for (id_t const id : ids)
			::djinn::scene_service::get_physics_object_manager()->destroy(id);
		return JS_UNDEFINED;
	}

	//
	//	SOUND EMITTER
	//
	JSValue create_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const sound_source_id = js::extract_id(ctx, argv[0]);
		sptr<sound_source> source = ::djinn::asset_service::get_sound_source_manager()->get(sound_source_id);
		id_t const instance_id = ::djinn::scene_service::get_sound_emitter_manager()->create(source);
		return js::create_id(ctx, instance_id);
	}
	JSValue start_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_sound_emitter_manager()->get(id)->start();
		return JS_UNDEFINED;
	}
	JSValue stop_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_sound_emitter_manager()->get(id)->stop();
		return JS_UNDEFINED;
	}
	JSValue set_emitter_volume(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const volume = js::extract_f32(ctx, argv[1]);
		::djinn::scene_service::get_sound_emitter_manager()->get(id)->set_volume(volume);
		return JS_UNDEFINED;
	}
	JSValue set_emitter_spatialization_enabled(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		bool const enabled = js::extract_bool(ctx, argv[1]);
		::djinn::scene_service::get_sound_emitter_manager()->get(id)->set_spatialization_enabled(enabled);
		return JS_UNDEFINED;
	}
	JSValue set_emitter_looping(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		bool const looping = js::extract_bool(ctx, argv[1]);
		::djinn::scene_service::get_sound_emitter_manager()->get(id)->set_looping(looping);
		return JS_UNDEFINED;
	}
	JSValue set_emitter_rolloff(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		::djinn::scene_service::get_sound_emitter_manager()->get(id)->set_rolloff(f);
		return JS_UNDEFINED;
	}
	JSValue set_emitter_min_dist(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		::djinn::scene_service::get_sound_emitter_manager()->get(id)->set_min_distance(f);
		return JS_UNDEFINED;
	}
	JSValue set_emitter_max_dist(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		f32 const f = js::extract_f32(ctx, argv[1]);
		::djinn::scene_service::get_sound_emitter_manager()->get(id)->set_max_distance(f);
		return JS_UNDEFINED;
	}
	JSValue set_emitter_attenuation(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		u32 const attenuation = js::extract_u32(ctx, argv[1]);
		::djinn::scene_service::get_sound_emitter_manager()->get(id)->set_attenuation_model((ma_attenuation_model)attenuation);
		return JS_UNDEFINED;
	}
	JSValue destroy_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::scene_service::get_sound_emitter_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue destroy_all_sound_emitter(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::vector<id_t> const& ids = js::extract_id_array(ctx, argv[0]);
		for (id_t const id : ids)
			::djinn::scene_service::get_sound_emitter_manager()->destroy(id);
		return JS_UNDEFINED;
	}

	//
	//	COMMON
	//
	JSValue copy_transform(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const src_id = js::extract_id(ctx, argv[0]);
		id_t const dst_id = js::extract_id(ctx, argv[1]);
		sptr<scene_object> src = ::djinn::scene_service::get_scene_object(src_id);
		sptr<scene_object> dst = ::djinn::scene_service::get_scene_object(dst_id);
		dst->copy_transform(src);
		return JS_UNDEFINED;
	}
	JSValue set_parent(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const src_id = js::extract_id(ctx, argv[0]);
		id_t const dst_id = js::extract_id(ctx, argv[1]);
		sptr<scene_object> src = ::djinn::scene_service::get_scene_object(src_id);
		if (dst_id != 0)
		{
			sptr<scene_object> dst = ::djinn::scene_service::get_scene_object(dst_id);

			if (::djinn::scene_service::get_physics_object_manager()->has(src_id))
			{
				scene_object const* parent = dst.get();
				while (parent)
				{
					ASSERT(!::djinn::scene_service::get_physics_object_manager()->has(parent->get_id()));
					parent = parent->get_parent();
				}
			}

			src->set_parent(dst.get());
		}
		else
		{
			src->set_parent(nullptr);
		}
		return JS_UNDEFINED;
	}
	JSValue get_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_values(ctx, argc, argv, &scene_object::get_pos);
	}
	JSValue get_pos_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_pos, 0);
	}
	JSValue get_pos_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_pos, 1);
	}
	JSValue get_pos_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_pos, 2);
	}
	JSValue get_rot(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(id);
		return js::create_f32_array(ctx, 3, so->get_rot().data());
	}
	JSValue get_rot_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_rot, 0);
	}
	JSValue get_rot_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_rot, 1);
	}
	JSValue get_rot_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_rot, 2);
	}
	JSValue get_scale(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_values(ctx, argc, argv, &scene_object::get_scale);
	}
	JSValue get_scale_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_scale, 0);
	}
	JSValue get_scale_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_scale, 1);
	}
	JSValue get_scale_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_scale, 2);
	}
	JSValue get_pos_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_values(ctx, argc, argv, &scene_object::get_pos_world);
	}
	JSValue get_pos_x_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_pos_world, 0);
	}
	JSValue get_pos_y_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_pos_world, 1);
	}
	JSValue get_pos_z_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_pos_world, 2);
	}
	JSValue get_rot_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(id);
		return js::create_f32_array(ctx, 3, so->get_rot_world().data());
	}
	JSValue get_rot_x_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_rot_world, 0);
	}
	JSValue get_rot_y_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_rot_world, 1);
	}
	JSValue get_rot_z_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_rot_world, 2);
	}
	JSValue get_scale_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_values(ctx, argc, argv, &scene_object::get_scale_world);
	}
	JSValue get_scale_x_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_scale_world, 0);
	}
	JSValue get_scale_y_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_scale_world, 1);
	}
	JSValue get_scale_z_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return get_value(ctx, argc, argv, &scene_object::get_scale_world, 2);
	}
	JSValue set_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values<point<space::PARENT>>(ctx, argc, argv, &scene_object::set_pos);
	}
	JSValue set_pos_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_pos_x);
	}
	JSValue set_pos_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_pos_y);
	}
	JSValue set_pos_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_pos_z);
	}
	JSValue set_rot(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& rot = js::extract_f32_array(ctx, argv[1]);
		ASSERT(rot.size() == 3);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(id);
		so->set_rot(rot[0], rot[1], rot[2]);
		return JS_UNDEFINED;
	}
	JSValue set_rot_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_rot_x);
	}
	JSValue set_rot_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_rot_y);
	}
	JSValue set_rot_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_rot_z);
	}
	JSValue set_scale(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values<vec<space::PARENT>>(ctx, argc, argv, &scene_object::set_scale);
	}
	JSValue set_scale_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_scale_x);
	}
	JSValue set_scale_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_scale_y);
	}
	JSValue set_scale_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_scale_z);
	}
	JSValue set_pos_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values<point<space::WORLD>>(ctx, argc, argv, &scene_object::set_pos_world);
	}
	JSValue set_pos_x_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_pos_x_world);
	}
	JSValue set_pos_y_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_pos_y_world);
	}
	JSValue set_pos_z_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_pos_z_world);
	}
	JSValue set_rot_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& rot = js::extract_f32_array(ctx, argv[1]);
		ASSERT(rot.size() == 3);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(id);
		so->set_rot_world(rot[0], rot[1], rot[2]);
		return JS_UNDEFINED;
	}
	JSValue set_rot_x_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_rot_x_world);
	}
	JSValue set_rot_y_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_rot_y_world);
	}
	JSValue set_rot_z_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_rot_z_world);
	}
	JSValue set_scale_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values<vec<space::WORLD>>(ctx, argc, argv, &scene_object::set_scale_world);
	}
	JSValue set_scale_x_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_scale_x_world);
	}
	JSValue set_scale_y_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_scale_y_world);
	}
	JSValue set_scale_z_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::set_scale_z_world);
	}
	JSValue add_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values<vec<space::PARENT>>(ctx, argc, argv, &scene_object::add_pos);
	}
	JSValue add_pos_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_pos_x);
	}
	JSValue add_pos_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_pos_y);
	}
	JSValue add_pos_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_pos_z);
	}
	JSValue add_rot(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& pos = js::extract_f32_array(ctx, argv[1]);
		ASSERT(pos.size() == 3);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(id);
		so->add_rot(pos[0], pos[1], pos[2]);
		return JS_UNDEFINED;
	}
	JSValue add_rot_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_rot_x);
	}
	JSValue add_rot_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_rot_y);
	}
	JSValue add_rot_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_rot_z);
	}
	JSValue add_scale(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values<vec<space::PARENT>>(ctx, argc, argv, &scene_object::add_scale);
	}
	JSValue add_scale_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_scale_x);
	}
	JSValue add_scale_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_scale_y);
	}
	JSValue add_scale_z(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_scale_z);
	}
	JSValue add_pos_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values<vec<space::WORLD>>(ctx, argc, argv, &scene_object::add_pos_world);
	}
	JSValue add_pos_x_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_pos_x_world);
	}
	JSValue add_pos_y_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_pos_y_world);
	}
	JSValue add_pos_z_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_pos_z_world);
	}
	JSValue add_rot_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& pos = js::extract_f32_array(ctx, argv[1]);
		ASSERT(pos.size() == 3);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(id);
		so->add_rot_world(pos[0], pos[1], pos[2]);
		return JS_UNDEFINED;
	}
	JSValue add_rot_x_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_rot_x_world);
	}
	JSValue add_rot_y_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_rot_y_world);
	}
	JSValue add_rot_z_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_rot_z_world);
	}
	JSValue add_scale_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_values<vec<space::WORLD>>(ctx, argc, argv, &scene_object::add_scale_world);
	}
	JSValue add_scale_x_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_scale_x_world);
	}
	JSValue add_scale_y_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_scale_y_world);
	}
	JSValue add_scale_z_world(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		return set_value(ctx, argc, argv, &scene_object::add_scale_z_world);
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
		// TAG
		{
			super::register_function(ctx, "Tag", "add", 2, js::scene_service::add_tag);
			super::register_function(ctx, "Tag", "has", 2, js::scene_service::has_tag);
			super::register_function(ctx, "Tag", "get", 1, js::scene_service::get_tags);
			super::register_function(ctx, "Tag", "remove", 2, js::scene_service::remove_tag);
		}
		// WAYPOINT
		{
			super::register_function(ctx, "Waypoint", "create", 0, js::scene_service::create_waypoint);
			super::register_function(ctx, "Waypoint", "destroy", 1, js::scene_service::destroy_waypoint);
			super::register_function(ctx, "Waypoint", "destroyAll", 1, js::scene_service::destroy_all_waypoint);
		}
		// LIGHT
		{
			super::register_function(ctx, "Light", "create", 0, js::scene_service::create_light);
			super::register_function(ctx, "Light", "setAmbient", 2, js::scene_service::set_light_color_ambient);
			super::register_function(ctx, "Light", "setDiffuse", 2, js::scene_service::set_light_color_diffuse);
			super::register_function(ctx, "Light", "setSpecular", 2, js::scene_service::set_light_color_specular);
			super::register_function(ctx, "Light", "destroy", 1, js::scene_service::destroy_light);
			super::register_function(ctx, "Light", "destroyAll", 1, js::scene_service::destroy_all_light);
		}
		// PHORM
		{
			super::register_function(ctx, "Phorm", "setShaders", 2, js::scene_service::set_phorm_shaders);
			super::register_function(ctx, "Phorm", "setAlphaShaders", 2, js::scene_service::set_phorm_alpha_shaders);
			super::register_function(ctx, "Phorm", "setVisible", 2, js::scene_service::set_phorm_visible);
			super::register_function(ctx, "Phorm", "destroy", 1, js::scene_service::destroy_phorm);
			super::register_function(ctx, "Phorm", "destroyAll", 1, js::scene_service::destroy_all_phorm);
		}
		// XPORT
		{
			super::register_function(ctx, "Xport", "load", 1, js::scene_service::load_xport);
			super::register_function(ctx, "Xport", "getName", 1, js::scene_service::get_xport_name);
		}
		// MESH INSTANCE
		{
			super::register_function(ctx, "MeshInstance", "create", 2, js::scene_service::create_mesh_instance);
			super::register_function(ctx, "MeshInstance", "setUniforms", 2, js::scene_service::set_mesh_instance_uniforms);
			super::register_function(ctx, "MeshInstance", "setVisible", 2, js::scene_service::set_mesh_instance_visible);
			super::register_function(ctx, "MeshInstance", "setAction", 3, js::scene_service::set_mesh_instance_action);
			super::register_function(ctx, "MeshInstance", "getBonePos", 2, js::scene_service::get_mesh_instance_bone_pos);
			super::register_function(ctx, "MeshInstance", "getBoneBasisX", 2, js::scene_service::get_mesh_instance_bone_basis_x);
			super::register_function(ctx, "MeshInstance", "getBoneBasisY", 2, js::scene_service::get_mesh_instance_bone_basis_y);
			super::register_function(ctx, "MeshInstance", "getBoneBasisZ", 2, js::scene_service::get_mesh_instance_bone_basis_z);
			super::register_function(ctx, "MeshInstance", "destroy", 1, js::scene_service::destroy_mesh_instance);
			super::register_function(ctx, "MeshInstance", "destroyAll", 1, js::scene_service::destroy_all_mesh_instance);
		}
		// ENTITY / CAMERA
		{
			super::register_function(ctx, "Entity", "load", MAX_VALUE_T(s32), js::scene_service::load_entity);
			super::register_function(ctx, "Entity", "destroy", 1, js::scene_service::destroy_entity);
			super::register_function(ctx, "Entity", "destroyAll", 1, js::scene_service::destroy_all_entity);
			super::register_function(ctx, "Camera", "load", MAX_VALUE_T(s32), js::scene_service::load_camera);
			super::register_function(ctx, "Camera", "configure", 5, js::scene_service::configure_camera);
			super::register_function(ctx, "Entity", "requestImGui", 1, js::scene_service::request_imgui);
		}
		// PHYSICS
		{
			super::register_function(ctx, "Physics", "bind", 2, js::scene_service::bind_physics_object);
			super::register_function(ctx, "Physics", "createBox", 3, js::scene_service::create_physics_box);
			super::register_function(ctx, "Physics", "createCylinder", 3, js::scene_service::create_physics_cylinder);
			super::register_function(ctx, "Physics", "createSphere", 3, js::scene_service::create_physics_sphere);
			super::register_function(ctx, "Physics", "createCapsuleX", 4, js::scene_service::create_physics_capsule_x);
			super::register_function(ctx, "Physics", "createCapsuleY", 4, js::scene_service::create_physics_capsule_y);
			super::register_function(ctx, "Physics", "createCapsuleZ", 4, js::scene_service::create_physics_capsule_z);
			super::register_function(ctx, "Physics", "createBVH", 1, js::scene_service::create_physics_bvh);
			super::register_function(ctx, "Physics", "setFriction", 2, js::scene_service::set_friction);
			super::register_function(ctx, "Physics", "getVelocity", 1, js::scene_service::get_velocity);
			super::register_function(ctx, "Physics", "getSpeed", 1, js::scene_service::get_speed);
			super::register_function(ctx, "Physics", "setVelocity", 2, js::scene_service::set_velocity);
			super::register_function(ctx, "Physics", "setVelocityX", 2, js::scene_service::set_velocity_x);
			super::register_function(ctx, "Physics", "setVelocityY", 2, js::scene_service::set_velocity_y);
			super::register_function(ctx, "Physics", "setVelocityZ", 2, js::scene_service::set_velocity_z);
			super::register_function(ctx, "Physics", "getVelocityWorld", 1, js::scene_service::get_velocity_world);
			super::register_function(ctx, "Physics", "getSpeedWorld", 1, js::scene_service::get_speed);
			super::register_function(ctx, "Physics", "setVelocityWorld", 2, js::scene_service::set_velocity_world);
			super::register_function(ctx, "Physics", "setVelocityXWorld", 2, js::scene_service::set_velocity_x_world);
			super::register_function(ctx, "Physics", "setVelocityYWorld", 2, js::scene_service::set_velocity_y_world);
			super::register_function(ctx, "Physics", "setVelocityZWorld", 2, js::scene_service::set_velocity_z_world);
			super::register_function(ctx, "Physics", "setAngularVelocity", 2, js::scene_service::set_angular_velocity);
			super::register_function(ctx, "Physics", "setAngularFactor", 2, js::scene_service::set_angular_factor);
			super::register_function(ctx, "Physics", "enableCollision", 1, js::scene_service::enable_collision);
			super::register_function(ctx, "Physics", "disableCollision", 1, js::scene_service::disable_collision);
			super::register_function(ctx, "Physics", "applyImpulse", 2, js::scene_service::apply_impulse);
			super::register_function(ctx, "Physics", "setDamping", 2, js::scene_service::set_damping);
			super::register_function(ctx, "Physics", "setAngularDamping", 2, js::scene_service::set_angular_damping);
			super::register_function(ctx, "Physics", "setMaxSpeedX", 2, js::scene_service::set_max_speed_x);
			super::register_function(ctx, "Physics", "setMaxSpeedY", 2, js::scene_service::set_max_speed_y);
			super::register_function(ctx, "Physics", "setMaxSpeedZ", 2, js::scene_service::set_max_speed_z);
			super::register_function(ctx, "Physics", "castRay", 3, js::scene_service::cast_ray);
			super::register_function(ctx, "Physics", "getNormalTangent", 3, js::scene_service::get_normal_tangent);
			super::register_function(ctx, "Physics", "setGravity", 2, js::scene_service::set_gravity);
			super::register_function(ctx, "Physics", "setKinematic", 2, js::scene_service::set_kinematic);
			super::register_function(ctx, "Physics", "setGhost", 2, js::scene_service::set_ghost);
			super::register_function(ctx, "Physics", "collideNSlide", 3, js::scene_service::collide_and_slide);
			super::register_function(ctx, "Physics", "aabbIntersects", 2, js::scene_service::aabb_intersects);
			super::register_function(ctx, "Physics", "destroy", 1, js::scene_service::destroy_physics_object);
			super::register_function(ctx, "Physics", "destroyAll", 1, js::scene_service::destroy_all_physics_object);
		}
		// SOUND EMITTER
		{
			super::register_function(ctx, "SoundEmitter", "create", 1, js::scene_service::create_emitter);
			super::register_function(ctx, "SoundEmitter", "start", 1, js::scene_service::start_emitter);
			super::register_function(ctx, "SoundEmitter", "stop", 1, js::scene_service::stop_emitter);
			super::register_function(ctx, "SoundEmitter", "setVolume", 2, js::scene_service::set_emitter_volume);
			super::register_function(ctx, "SoundEmitter", "setSpatializationEnabled", 2, js::scene_service::set_emitter_spatialization_enabled);
			super::register_function(ctx, "SoundEmitter", "setLooping", 2, js::scene_service::set_emitter_looping);
			super::register_function(ctx, "SoundEmitter", "setRolloff", 2, js::scene_service::set_emitter_rolloff);
			super::register_function(ctx, "SoundEmitter", "setMinDistance", 2, js::scene_service::set_emitter_min_dist);
			super::register_function(ctx, "SoundEmitter", "setMaxDistance", 2, js::scene_service::set_emitter_max_dist);
			super::register_function(ctx, "SoundEmitter", "setAttenutation", 2, js::scene_service::set_emitter_attenuation);
			super::register_function(ctx, "SoundEmitter", "destroy", 1, js::scene_service::destroy_emitter);
			super::register_function(ctx, "SoundEmitter", "destroyAll", 1, js::scene_service::destroy_all_sound_emitter);
		}
		// COMMON
		{
			super::register_function(ctx, "copyTransform", 2, js::scene_service::copy_transform);
			super::register_function(ctx, "setParent", 2, js::scene_service::set_parent);
			super::register_function(ctx, "getPos", 1, js::scene_service::get_pos);
			super::register_function(ctx, "getPosX", 1, js::scene_service::get_pos_x);
			super::register_function(ctx, "getPosY", 1, js::scene_service::get_pos_y);
			super::register_function(ctx, "getPosZ", 1, js::scene_service::get_pos_z);
			super::register_function(ctx, "getRot", 1, js::scene_service::get_rot);
			super::register_function(ctx, "getRotX", 1, js::scene_service::get_rot_x);
			super::register_function(ctx, "getRotY", 1, js::scene_service::get_rot_y);
			super::register_function(ctx, "getRotZ", 1, js::scene_service::get_rot_z);
			super::register_function(ctx, "getScale", 1, js::scene_service::get_scale);
			super::register_function(ctx, "getScaleX", 1, js::scene_service::get_scale_x);
			super::register_function(ctx, "getScaleY", 1, js::scene_service::get_scale_y);
			super::register_function(ctx, "getScaleZ", 1, js::scene_service::get_scale_z);
			super::register_function(ctx, "getPosWorld", 1, js::scene_service::get_pos_world);
			super::register_function(ctx, "getPosXWorld", 1, js::scene_service::get_pos_x_world);
			super::register_function(ctx, "getPosYWorld", 1, js::scene_service::get_pos_y_world);
			super::register_function(ctx, "getPosZWorld", 1, js::scene_service::get_pos_z_world);
			super::register_function(ctx, "getRotWorld", 1, js::scene_service::get_rot_world);
			super::register_function(ctx, "getRotXWorld", 1, js::scene_service::get_rot_x_world);
			super::register_function(ctx, "getRotYWorld", 1, js::scene_service::get_rot_y_world);
			super::register_function(ctx, "getRotZWorld", 1, js::scene_service::get_rot_z_world);
			super::register_function(ctx, "getScaleWorld", 1, js::scene_service::get_scale_world);
			super::register_function(ctx, "getScaleXWorld", 1, js::scene_service::get_scale_x_world);
			super::register_function(ctx, "getScaleYWorld", 1, js::scene_service::get_scale_y_world);
			super::register_function(ctx, "getScaleZWorld", 1, js::scene_service::get_scale_z_world);
			super::register_function(ctx, "setPos", 1, js::scene_service::set_pos);
			super::register_function(ctx, "setPosX", 1, js::scene_service::set_pos_x);
			super::register_function(ctx, "setPosY", 1, js::scene_service::set_pos_y);
			super::register_function(ctx, "setPosZ", 1, js::scene_service::set_pos_z);
			super::register_function(ctx, "setRot", 1, js::scene_service::set_rot);
			super::register_function(ctx, "setRotX", 1, js::scene_service::set_rot_x);
			super::register_function(ctx, "setRotY", 1, js::scene_service::set_rot_y);
			super::register_function(ctx, "setRotZ", 1, js::scene_service::set_rot_z);
			super::register_function(ctx, "setScale", 1, js::scene_service::set_scale);
			super::register_function(ctx, "setScaleX", 1, js::scene_service::set_scale_x);
			super::register_function(ctx, "setScaleY", 1, js::scene_service::set_scale_y);
			super::register_function(ctx, "setScaleZ", 1, js::scene_service::set_scale_z);
			super::register_function(ctx, "setPosWorld", 1, js::scene_service::set_pos_world);
			super::register_function(ctx, "setPosXWorld", 1, js::scene_service::set_pos_x_world);
			super::register_function(ctx, "setPosYWorld", 1, js::scene_service::set_pos_y_world);
			super::register_function(ctx, "setPosZWorld", 1, js::scene_service::set_pos_z_world);
			super::register_function(ctx, "setRotWorld", 1, js::scene_service::set_rot_world);
			super::register_function(ctx, "setRotXWorld", 1, js::scene_service::set_rot_x_world);
			super::register_function(ctx, "setRotYWorld", 1, js::scene_service::set_rot_y_world);
			super::register_function(ctx, "setRotZWorld", 1, js::scene_service::set_rot_z_world);
			super::register_function(ctx, "setScaleWorld", 1, js::scene_service::set_scale_world);
			super::register_function(ctx, "setScaleXWorld", 1, js::scene_service::set_scale_x_world);
			super::register_function(ctx, "setScaleYWorld", 1, js::scene_service::set_scale_y_world);
			super::register_function(ctx, "setScaleZWorld", 1, js::scene_service::set_scale_z_world);
			super::register_function(ctx, "addPos", 1, js::scene_service::add_pos);
			super::register_function(ctx, "addPosX", 1, js::scene_service::add_pos_x);
			super::register_function(ctx, "addPosY", 1, js::scene_service::add_pos_y);
			super::register_function(ctx, "addPosZ", 1, js::scene_service::add_pos_z);
			super::register_function(ctx, "addRot", 1, js::scene_service::add_rot);
			super::register_function(ctx, "addRotX", 1, js::scene_service::add_rot_x);
			super::register_function(ctx, "addRotY", 1, js::scene_service::add_rot_y);
			super::register_function(ctx, "addRotZ", 1, js::scene_service::add_rot_z);
			super::register_function(ctx, "addScale", 1, js::scene_service::add_scale);
			super::register_function(ctx, "addScaleX", 1, js::scene_service::add_scale_x);
			super::register_function(ctx, "addScaleY", 1, js::scene_service::add_scale_y);
			super::register_function(ctx, "addScaleZ", 1, js::scene_service::add_scale_z);
			super::register_function(ctx, "addPosWorld", 1, js::scene_service::add_pos_world);
			super::register_function(ctx, "addPosXWorld", 1, js::scene_service::add_pos_x_world);
			super::register_function(ctx, "addPosYWorld", 1, js::scene_service::add_pos_y_world);
			super::register_function(ctx, "addPosZWorld", 1, js::scene_service::add_pos_z_world);
			super::register_function(ctx, "addRotWorld", 1, js::scene_service::add_rot_world);
			super::register_function(ctx, "addRotXWorld", 1, js::scene_service::add_rot_x_world);
			super::register_function(ctx, "addRotYWorld", 1, js::scene_service::add_rot_y_world);
			super::register_function(ctx, "addRotZWorld", 1, js::scene_service::add_rot_z_world);
			super::register_function(ctx, "addScaleWorld", 1, js::scene_service::add_scale_world);
			super::register_function(ctx, "addScaleXWorld", 1, js::scene_service::add_scale_x_world);
			super::register_function(ctx, "addScaleYWorld", 1, js::scene_service::add_scale_y_world);
			super::register_function(ctx, "addScaleZWorld", 1, js::scene_service::add_scale_z_world);
		}
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
	waypoint_manager* scene_service::get_waypoint_manager()
	{
		return &s_instance->m_waypoint_manager;
	}
	sound_emitter_manager* scene_service::get_sound_emitter_manager()
	{
		return &s_instance->m_emitter_manager;
	}
	JSRuntime* scene_service::get_runtime()
	{
		return s_instance->m_runtime;
	}
	void scene_service::update(f32 const dt, f32 const time)
	{
		// call __main()
		s_instance->m_entity_manager.for_each([&](sptr<entity> e, id_t const id)
			{
				e->update(dt, time);
			});
		s_instance->m_camera_entity_manager.for_each([&](sptr<camera_entity> cam, id_t const id)
			{
				cam->update(dt, time);
			});
		s_instance->m_physics_object_manager.update(dt);

		s_instance->m_emitter_manager.for_each([](sptr<sound_emitter> emitter, id_t const id)
			{
				emitter->update_from_scene_object();
			});
	}
	void scene_service::draw()
	{
		// call __draw()
		s_instance->m_entity_manager.for_each([](sptr<entity> e, id_t const id)
			{
				e->call_draw();
			});
		s_instance->m_camera_entity_manager.for_each([](sptr<camera_entity> e, id_t const id)
			{
				e->call_draw();
			});
		// now that all script functions for this frame have run (__main() and __draw()), compute final light and camera transforms
		s_instance->m_light_manager.update();
		s_instance->m_light_manager.bind();
		s_instance->m_camera_entity_manager.for_each([&](sptr<camera_entity> cam, id_t const id)
			{
				cam->update_mats();
			});
		// finally, render everything
		s_instance->m_phorm_manager.for_each([](sptr<phorm> p, id_t const id)
			{
				p->draw(render_service::get_context());
			});
	}
	void scene_service::draw_ui()
	{
		s_instance->m_entity_manager.for_each([](sptr<entity> e, id_t const id)
			{
				e->call_ui();
			});
		s_instance->m_camera_entity_manager.for_each([](sptr<camera_entity> e, id_t const id)
			{
				e->call_ui();
			});
	}
	void scene_service::draw_imgui()
	{
		s_instance->m_entity_manager.for_each([](sptr<entity> e, id_t const id)
			{
				e->call_imgui();
			});
		s_instance->m_camera_entity_manager.for_each([](sptr<camera_entity> e, id_t const id)
			{
				e->call_imgui();
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
		else if (get_waypoint_manager()->has(id))
			return get_waypoint_manager()->get(id);
		else if (get_sound_emitter_manager()->has(id))
			return get_sound_emitter_manager()->get(id);
		return get_mesh_instance_manager()->get(id);
	}
	tagged* scene_service::get_tagged(id_t const id)
	{
		if (get_entity_manager()->has(id))
			return get_entity_manager()->get(id).get();
		else if (get_camera_entity_manager()->has(id))
			return get_camera_entity_manager()->get(id).get();
		else if (get_phorm_manager()->has(id))
			return get_phorm_manager()->get(id).get();
		else if (get_light_manager()->has(id))
			return get_light_manager()->get(id).get();
		return get_waypoint_manager()->get(id).get();
	}
	xport* scene_service::get_xport(id_t const id)
	{
		if (get_phorm_manager()->has(id))
			return get_phorm_manager()->get(id).get();
		else if (get_light_manager()->has(id))
			return get_light_manager()->get(id).get();
		return get_waypoint_manager()->get(id).get();
	}



	scene_service::scene_service() :
		haul::parent<service<scene_service>>("Scene"),
		m_runtime(JS_NewRuntime()),
		m_entity_manager(m_runtime),
		m_camera_entity_manager(m_runtime)
	{}
} // namespace djinn

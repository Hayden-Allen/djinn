#include "pch.h"
#include "asset_service.h"
#include "script/js.h"
#include "scene/camera_entity.h"
#include "scene_service.h"
#include "core/constants.h"

namespace djinn::js::asset_service
{
	static texture_options parse_texture_options(JSContext* const ctx, JSValue const& val)
	{
		texture_options options;
		std::unordered_map<std::string, JSValue> const& map = js::extract_map(ctx, val);
		for (auto const& pair : map)
		{
			if (pair.first == "minFilter")
				options.min_filter = js::extract_u32(ctx, pair.second);
			else if (pair.first == "magFilter")
				options.mag_filter = js::extract_u32(ctx, pair.second);
			else if (pair.first == "wrapS")
				options.wrap_t = js::extract_u32(ctx, pair.second);
			else if (pair.first == "wrapT")
				options.wrap_t = js::extract_u32(ctx, pair.second);
			else if (pair.first == "wrapR")
				options.wrap_r = js::extract_u32(ctx, pair.second);
			else
				ASSERT(false);
		}
		return options;
	}
	static sptr<texture> get_texture(id_t const id)
	{
		if (::djinn::asset_service::get_texture_manager()->has(id))
			return ::djinn::asset_service::get_texture_manager()->get(id);
		return ::djinn::asset_service::get_cubemap_manager()->get(id);
	}


	JSValue create_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 4);
		u32 const vertex_count = js::extract_u32(ctx, argv[0]);
		std::vector<u32> const& vertex_layout = js::extract_u32_array(ctx, argv[1]);
		u32 const index_count = js::extract_u32(ctx, argv[2]);
		std::vector<id_t> const& texture_ids = js::extract_id_array(ctx, argv[3]);

		std::vector<wptr<texture>> textures;
		textures.reserve(texture_ids.size());
		for (id_t const id : texture_ids)
			textures.emplace_back(get_texture(id));

		JSValue ret = js::create_id(ctx, ::djinn::asset_service::get_mesh_manager()->create(vertex_count, vertex_layout, index_count, textures));
		return ret;
	}
	JSValue destroy_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::asset_service::get_mesh_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue update_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 3);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<f32> const& vertices = js::extract_f32_array(ctx, argv[1]);
		std::vector<u32> const& indices = js::extract_u32_array(ctx, argv[2]);
		::djinn::asset_service::get_mesh_manager()->update(id, vertices, indices);
		return JS_UNDEFINED;
	}
	JSValue create_shader(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		std::string const vert_src = js::extract_string(ctx, argv[0]);
		std::string const frag_src = js::extract_string(ctx, argv[1]);
		return js::create_id(ctx, ::djinn::asset_service::get_shader_manager()->create(vert_src, frag_src));
	}
	extern JSValue load_shader(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		std::string const vert_fp = js::extract_string(ctx, argv[0]);
		std::string const frag_fp = js::extract_string(ctx, argv[1]);
		return js::create_id(ctx, ::djinn::asset_service::get_shader_manager()->load(vert_fp, frag_fp));
	}
	JSValue destroy_shader(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t id = js::extract_id(ctx, argv[0]);
		::djinn::asset_service::get_shader_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue set_shader_uniforms(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::unordered_map<std::string, JSValue> const& map = js::extract_map(ctx, argv[1]);
		for (auto const& pair : map)
			::djinn::asset_service::get_shader_manager()->set_uniform(ctx, id, pair.first, pair.second);
		return JS_UNDEFINED;
	}
	JSValue set_shader_camera_uniforms(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const shader_id = js::extract_id(ctx, argv[0]);
		id_t const camera_id = js::extract_id(ctx, argv[1]);
		sptr<camera_entity> const cam = ::djinn::scene_service::get_camera_entity_manager()->get(camera_id);
		::djinn::asset_service::get_shader_manager()->set_uniform_mat4(shader_id, c::uniform::view_mat, cam->get_view().e);
		::djinn::asset_service::get_shader_manager()->set_uniform_mat4(shader_id, c::uniform::proj_mat, cam->get_proj().e);
		::djinn::asset_service::get_shader_manager()->set_uniform_mat4(shader_id, c::uniform::vp_mat, cam->get_view_proj().e);
		// remove translation from view matrix
		mat<space::WORLD, space::CLIP> const& vpr = cam->get_proj() * cam->get_view().basis_copy();
		::djinn::asset_service::get_shader_manager()->set_uniform_mat4(shader_id, c::uniform::vpr_mat, vpr.e);
		return JS_UNDEFINED;
	}
	JSValue create_texture(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2 || argc == 3);
		u32 const width = js::extract_u32(ctx, argv[0]);
		u32 const height = js::extract_u32(ctx, argv[1]);
		texture_options options;
		if (argc == 3)
			options = parse_texture_options(ctx, argv[2]);
		return js::create_id(ctx, ::djinn::asset_service::get_texture_manager()->create(width, height, options));
	}
	JSValue load_texture(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1 || argc == 2);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		texture_options options;
		if (argc == 2)
			options = parse_texture_options(ctx, argv[1]);
		return js::create_id(ctx, ::djinn::asset_service::get_texture_manager()->load(fp, options));
	}
	JSValue destroy_texture(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::asset_service::get_texture_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue update_texture(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2 || argc == 3);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::vector<u8> const& subpixels = js::extract_u8_array(ctx, argv[1]);
		if (argc == 3)
		{
			texture_options options = parse_texture_options(ctx, argv[2]);
			::djinn::asset_service::get_texture_manager()->update(id, subpixels, options);
		}
		else
			::djinn::asset_service::get_texture_manager()->update(id, subpixels);
		return JS_UNDEFINED;
	}
	JSValue create_cubemap(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2 || argc == 3);
		u32 const width = js::extract_u32(ctx, argv[0]);
		u32 const height = js::extract_u32(ctx, argv[1]);
		texture_options options;
		if (argc == 3)
			options = parse_texture_options(ctx, argv[2]);
		return js::create_id(ctx, ::djinn::asset_service::get_cubemap_manager()->create(width, height, options));
	}
	JSValue load_cubemap(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1 || argc == 2);
		std::vector<std::string> const& fps = js::extract_string_array(ctx, argv[0]);
		ASSERT(fps.size() == 6);
		std::array<std::string, 6> fps_arr;
		for (s32 i = 0; i < 6; i++)
			fps_arr[i] = fps[i];
		texture_options options;
		if (argc == 2)
			options = parse_texture_options(ctx, argv[1]);
		return js::create_id(ctx, ::djinn::asset_service::get_cubemap_manager()->load(fps_arr, options));
	}
	JSValue destroy_cubemap(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::asset_service::get_cubemap_manager()->destroy(id);
		return JS_UNDEFINED;
	}
	JSValue update_cubemap(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2 || argc == 3);
		id_t const id = js::extract_id(ctx, argv[0]);
		std::array<std::vector<u8>, 6> subpixels;
		ASSERT(js::extract_array_length(ctx, argv[1]) == 6);
		js::array_for(ctx, argv[1], [&](JSValue const& arr, u32 const i)
			{
				subpixels[i] = std::move(js::extract_u8_array(ctx, arr));
			});

		if (argc == 3)
		{
			texture_options options = parse_texture_options(ctx, argv[2]);
			::djinn::asset_service::get_cubemap_manager()->update(id, subpixels, options);
		}
		else
			::djinn::asset_service::get_cubemap_manager()->update(id, subpixels);
		return JS_UNDEFINED;
	}
	JSValue load_sound_source(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& fp = js::extract_string(ctx, argv[0]);
		return js::create_id(ctx, ::djinn::asset_service::get_sound_source_manager()->load(fp));
	}
	JSValue destroy_sound_source(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::asset_service::get_sound_source_manager()->destroy(id);
		return JS_UNDEFINED;
	}
} // namespace djinn::js::asset_service



namespace djinn
{
	void asset_service::init()
	{
		ASSERT(!s_instance);
		s_instance = new asset_service();
	}
	void asset_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "Mesh", "create", 4, js::asset_service::create_mesh);
		super::register_function(ctx, "Mesh", "update", 3, js::asset_service::update_mesh);
		super::register_function(ctx, "Mesh", "destroy", 1, js::asset_service::destroy_mesh);
		super::register_function(ctx, "Shader", "create", 2, js::asset_service::create_shader);
		super::register_function(ctx, "Shader", "load", 2, js::asset_service::load_shader);
		super::register_function(ctx, "Shader", "destroy", 1, js::asset_service::destroy_shader);
		super::register_function(ctx, "Shader", "setUniforms", 2, js::asset_service::set_shader_uniforms);
		super::register_function(ctx, "Shader", "setCameraUniforms", 2, js::asset_service::set_shader_camera_uniforms);
		super::register_function(ctx, "Texture", "create", 3, js::asset_service::create_texture);
		super::register_function(ctx, "Texture", "load", 2, js::asset_service::load_texture);
		super::register_function(ctx, "Texture", "destroy", 1, js::asset_service::destroy_texture);
		super::register_function(ctx, "Texture", "update", 3, js::asset_service::update_texture);
		super::register_function(ctx, "Cubemap", "create", 3, js::asset_service::create_cubemap);
		super::register_function(ctx, "Cubemap", "load", 2, js::asset_service::load_cubemap);
		super::register_function(ctx, "Cubemap", "destroy", 1, js::asset_service::destroy_cubemap);
		super::register_function(ctx, "Cubemap", "update", 3, js::asset_service::update_cubemap);
		super::register_function(ctx, "Sound", "load", 1, js::asset_service::load_sound_source);
		super::register_function(ctx, "Sound", "destroy", 1, js::asset_service::destroy_sound_source);
	}
	mesh_manager* asset_service::get_mesh_manager()
	{
		return &s_instance->m_mesh_manager;
	}
	shader_manager* asset_service::get_shader_manager()
	{
		return &s_instance->m_shader_manager;
	}
	texture_manager* asset_service::get_texture_manager()
	{
		return &s_instance->m_texture_manager;
	}
	cubemap_manager* asset_service::get_cubemap_manager()
	{
		return &s_instance->m_cubemap_manager;
	}
	sound_source_manager* asset_service::get_sound_source_manager()
	{
		return &s_instance->m_sound_source_manager;
	}



	asset_service::asset_service() :
		haul::parent<service<asset_service>>("Asset")
	{}
} // namespace djinn

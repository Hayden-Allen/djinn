#include "pch.h"
#include "asset_service.h"
#include "js.h"

namespace djinn
{
	namespace js::asset_service
	{
		/*s32 init(JSContext* const ctx, JSModuleDef* const m)
		{
			return JS_SetModuleExportList(ctx, m, s_fns, s_FN_COUNT);
		}*/
		JSValue create_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 3);
			u32 vertex_count = js::extract_u32(ctx, argv[0]);
			std::vector<u32> const& vertex_layout = js::extract_u32_array(ctx, argv[1]);
			u32 index_count = js::extract_u32(ctx, argv[2]);

			return js::create_id(ctx, ::djinn::asset_service::get_mesh_manager()->create(vertex_count, vertex_layout, index_count));
		}
		JSValue destroy_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 1);
			id_t id = js::extract_id(ctx, argv[0]);
			::djinn::asset_service::get_mesh_manager()->destroy(id);
			return JS_UNDEFINED;
		}
		JSValue update_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 3);
			id_t id = js::extract_id(ctx, argv[0]);
			std::vector<f32> const& vertices = js::extract_f32_array(ctx, argv[1]);
			std::vector<u32> const& indices = js::extract_u32_array(ctx, argv[2]);

			sptr<static_render_object> ro = ::djinn::asset_service::get_mesh_manager()->get(id);
			ro->get_vertex_array().get_vertices().update(vertices.data(), (u32)vertices.size(), 0);
			ro->get_indices().update(indices.data(), (u32)indices.size(), 0);
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
			id_t id = js::extract_id(ctx, argv[0]);
			std::unordered_map<std::string, JSValue> const& map = js::extract_map(ctx, argv[1]);
			for (auto const& pair : map)
				::djinn::asset_service::get_shader_manager()->set_uniform(ctx, id, pair.first, pair.second);
			return JS_UNDEFINED;
		}
	} // namespace js::asset_service



	void asset_service::init()
	{
		s_instance = new asset_service();
	}
	void asset_service::register_functions(JSContext* const ctx)
	{
		/*JSModuleDef* m;
		m = JS_NewCModule(ctx, "asset_service", js::asset_service::init);
		ASSERT(m);
		JS_AddModuleExportList(ctx, m, js::asset_service::s_fns, js::asset_service::s_FN_COUNT);*/
		super::register_function(ctx, "Mesh", "create", 3, js::asset_service::create_mesh);
		super::register_function(ctx, "Mesh", "destroy", 1, js::asset_service::destroy_mesh);
		super::register_function(ctx, "Mesh", "update", 3, js::asset_service::update_mesh);
		super::register_function(ctx, "Shader", "create", 2, js::asset_service::create_shader);
		super::register_function(ctx, "Shader", "load", 2, js::asset_service::load_shader);
		super::register_function(ctx, "Shader", "destroy", 2, js::asset_service::destroy_shader);
		super::register_function(ctx, "Shader", "setUniforms", 2, js::asset_service::set_shader_uniforms);
	}
	mesh_manager* asset_service::get_mesh_manager()
	{
		return &s_instance->m_mesh_manager;
	}
	shader_manager* asset_service::get_shader_manager()
	{
		return &s_instance->m_shader_manager;
	}



	asset_service::asset_service() :
		haul::parent<service<asset_service>>("Asset")
	{}
} // namespace djinn

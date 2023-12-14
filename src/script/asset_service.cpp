#include "pch.h"
#include "asset_service.h"

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
			u32 vertex_count, index_count;
			if (!u::extract_u32(ctx, argv[0], &vertex_count))
				return JS_EXCEPTION;

			std::vector<u32> const& vertex_layout = u::extract_u32_array(ctx, argv[1]);

			if (!u::extract_u32(ctx, argv[2], &index_count))
				return JS_EXCEPTION;

			return JS_NewUint32(ctx, ::djinn::asset_service::get_instance()->get_mesh_manager().create(vertex_count, vertex_layout, index_count));
		}
		JSValue destroy_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 1);
			id_t id;
			if (!u::extract_u32(ctx, argv[0], &id))
				return JS_EXCEPTION;
			::djinn::asset_service::get_instance()->get_mesh_manager().remove(id);
			return JS_UNDEFINED;
		}
		JSValue update_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 3);
			id_t id;
			if (!u::extract_u32(ctx, argv[0], &id))
				return JS_EXCEPTION;
			std::vector<f32> const& vertices = u::extract_f32_array(ctx, argv[1]);
			std::vector<u32> const& indices = u::extract_u32_array(ctx, argv[2]);
			sptr<static_render_object> ro = ::djinn::asset_service::get_instance()->get_mesh_manager().get(id);
			ro->get_vertex_array().get_vertices().update(vertices.data(), (u32)vertices.size(), 0);
			ro->get_indices().update(indices.data(), (u32)indices.size(), 0);
			return JS_UNDEFINED;
		}
		JSValue create_shader(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 2);
			std::string const vert_src = u::extract_string(ctx, argv[0]);
			std::string const frag_src = u::extract_string(ctx, argv[1]);
			return JS_NewUint32(ctx, ::djinn::asset_service::get_instance()->get_shader_manager().create(vert_src, frag_src));
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
		super::register_function(ctx, "create_mesh", 3, js::asset_service::create_mesh);
		super::register_function(ctx, "destroy_mesh", 1, js::asset_service::destroy_mesh);
		super::register_function(ctx, "update_mesh", 3, js::asset_service::update_mesh);
		super::register_function(ctx, "create_shader", 2, js::asset_service::create_shader);

	}
	mesh_manager& asset_service::get_mesh_manager()
	{
		return m_mesh_manager;
	}
	shader_manager& asset_service::get_shader_manager()
	{
		return m_shader_manager;
	}



	asset_service::asset_service() :
		haul::parent<service<asset_service>>()
	{}
} // namespace djinn

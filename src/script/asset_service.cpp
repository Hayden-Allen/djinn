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

			return JS_NewUint32(ctx, ::djinn::asset_service::get_instance()->create_mesh(vertex_count, {}, index_count));
		}
		JSValue destroy_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
		{
			ASSERT(argc == 1);
			id_t id;
			if (!u::extract_u32(ctx, argv[0], &id))
				return JS_EXCEPTION;
			::djinn::asset_service::get_instance()->destroy_mesh(id);
			return JS_UNDEFINED;
		}
	} // namespace js::asset_service



	void asset_service::register_functions(JSContext* const ctx)
	{
		/*JSModuleDef* m;
		m = JS_NewCModule(ctx, "asset_service", js::asset_service::init);
		ASSERT(m);
		JS_AddModuleExportList(ctx, m, js::asset_service::s_fns, js::asset_service::s_FN_COUNT);*/
		super::register_function(ctx, "create_mesh", 3, js::asset_service::create_mesh);
		super::register_function(ctx, "destroy_mesh", 1, js::asset_service::destroy_mesh);

	}
	id_t asset_service::create_mesh(u32 const vertex_count, std::vector<u32> const& vertex_layout, u32 const index_count)
	{
		return m_mesh_manager.create(vertex_count, vertex_layout, index_count);
	}
	void asset_service::destroy_mesh(id_t const id)
	{
		m_mesh_manager.remove(id);
	}



	asset_service::asset_service() :
		haul::parent<service<asset_service>>()
	{}
} // namespace djinn

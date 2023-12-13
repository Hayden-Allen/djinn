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
		JSValue create_mesh(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv)
		{
			ASSERT(argc == 3);
			id_t vertex_count, index_count;
			if (JS_ToUint32(ctx, &vertex_count, argv[0]))
			{
				ASSERT(false);
				return JS_EXCEPTION;
			}

			std::vector<u32> const& vertex_layout = u::extract_uint32_array(ctx, argv[1]);

			if (JS_ToUint32(ctx, &index_count, argv[2]))
			{
				ASSERT(false);
				return JS_EXCEPTION;
			}

			return JS_NewUint32(ctx, ::djinn::asset_service::get_instance()->create_mesh(vertex_count, {}, index_count));
		}
	} // namespace js::asset_service



	asset_service::~asset_service()
	{}



	asset_service* asset_service::get_instance()
	{
		if (!s_instance)
			s_instance = new asset_service();
		return s_instance;
	}
	void asset_service::shutdown()
	{
		ASSERT(s_instance);
		delete s_instance;
	}
	void asset_service::register_functions(JSContext* const ctx)
	{
		/*JSModuleDef* m;
		m = JS_NewCModule(ctx, "asset_service", js::asset_service::init);
		ASSERT(m);
		JS_AddModuleExportList(ctx, m, js::asset_service::s_fns, js::asset_service::s_FN_COUNT);*/
		JSValue const global = JS_GetGlobalObject(ctx);
		JSValue const fn = JS_NewCFunction(ctx, js::asset_service::create_mesh, "create_mesh", 3);
		JS_SetPropertyStr(ctx, global, "create_mesh", fn);
		JS_FreeValue(ctx, global);
	}
	id_t asset_service::create_mesh(u32 const vertex_count, std::vector<u32> const& vertex_layout, u32 const index_count)
	{
		return m_mesh_manager.create(vertex_count, vertex_layout, index_count);
	}



	asset_service::asset_service()
	{
		ASSERT(!s_instance);
	}
} // namespace djinn

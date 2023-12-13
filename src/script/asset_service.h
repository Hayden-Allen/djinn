#pragma once
#include "pch.h"
#include "service.h"
#include "mesh_manager.h"

namespace djinn
{
	namespace js::asset_service
	{
		extern JSValue create_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue destroy_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		/*static constexpr u64 s_FN_COUNT = 1;
		static inline const JSCFunctionListEntry s_fns[s_FN_COUNT] = {
			JS_CFUNC_DEF("create_mesh", 3, create_mesh)
		};
		extern s32 init(JSContext* const ctx, JSModuleDef* const m);*/
	} // namespace js::asset_service

	class asset_service final : public haul::parent<service<asset_service>>
	{
		friend class super;
	public:
		DCM(asset_service);
	public:
		static void register_functions(JSContext* const ctx);
		id_t create_mesh(u32 const vertex_count, std::vector<u32> const& vertex_layout, u32 const index_count);
		void destroy_mesh(id_t const id);
	private:
		static inline asset_service* s_instance = nullptr;
	private:
		mesh_manager m_mesh_manager;
	private:
		asset_service();
	};
} // namespace djinn

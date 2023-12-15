#pragma once
#include "pch.h"
#include "service.h"
#include "mesh_manager.h"
#include "shader_manager.h"

namespace djinn
{
	namespace js::asset_service
	{
		extern JSValue create_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue destroy_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue update_mesh(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue create_shader(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue destroy_shader(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue set_shader_uniforms(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
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
		static void init();
		static void register_functions(JSContext* const ctx);
		static mesh_manager& get_mesh_manager();
		static shader_manager& get_shader_manager();
	private:
		mesh_manager m_mesh_manager;
		shader_manager m_shader_manager;
	private:
		asset_service();
	};
} // namespace djinn

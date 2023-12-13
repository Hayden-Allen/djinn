#include "pch.h"
#include "mesh_manager.h"

namespace djinn
{
	namespace js::asset_service
	{
		extern JSValue create_mesh(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv);

		/*static constexpr u64 s_FN_COUNT = 1;
		static inline const JSCFunctionListEntry s_fns[s_FN_COUNT] = {
			JS_CFUNC_DEF("create_mesh", 3, create_mesh)
		};
		extern s32 init(JSContext* const ctx, JSModuleDef* const m);*/
	} // namespace js::asset_service

	class asset_service final
	{
	public:
		DCM(asset_service);
		~asset_service();
	public:
		static asset_service* get_instance();
		static void shutdown();
		static void register_functions(JSContext* const ctx);
		id_t create_mesh(u32 const vertex_count, std::vector<u32> const& vertex_layout, u32 const index_count);
	private:
		static inline asset_service* s_instance = nullptr;
	private:
		mesh_manager m_mesh_manager;
	private:
		asset_service();
	};
} // namespace djinn

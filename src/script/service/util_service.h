#pragma once
#include "pch.h"
#include "service.h"

namespace djinn
{
	namespace js::util_service
	{
		extern JSValue make_texture_path(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue list_files(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
	} // namespace js::util_service



	class util_service final : public haul::parent<service<util_service>>
	{
		friend class super;
	public:
		DCM(util_service);
	public:
		static void init();
		static void register_functions(JSContext* const ctx);
	private:
		util_service();
	};
} // namespace djinn

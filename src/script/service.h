#pragma once 
#include "pch.h"

namespace djinn
{
	// base class for all script services
	// all services X should be defined as:
	//	class X final : public service<X>
	template<typename DERIVED>
	class service
	{
	public:
		DCM(service);
		virtual ~service() {}
	public:
		static DERIVED* get_instance()
		{
			if (!s_instance)
				s_instance = new DERIVED();
			return s_instance;
		}
		static void shutdown()
		{
			ASSERT(s_instance);
			delete s_instance;
		}
		static void register_functions(JSContext* const ctx)
		{
			ASSERT(false);	// must be implemented by derived services
		}
	protected:
		service()
		{
			ASSERT(!s_instance);
		}
	protected:
		static void register_function(JSContext* const ctx, std::string const& name, s32 const num_args, JSCFunction* const fn)
		{
			JSValue const global = JS_GetGlobalObject(ctx);
			JSValue const fn_obj = JS_NewCFunction(ctx, fn, name.c_str(), num_args);
			JS_SetPropertyStr(ctx, global, name.c_str(), fn_obj);
			JS_FreeValue(ctx, global);
		}
	private:
		static inline DERIVED* s_instance = nullptr;
	};
}
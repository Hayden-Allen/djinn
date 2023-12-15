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
		static DERIVED* get()
		{
			return s_instance;
		}
		static void init()
		{
			ASSERT(false); // must be implemented by derived services
		}
		static void shutdown()
		{
			ASSERT(s_instance);
			delete s_instance;
		}
		static void register_functions(JSContext* const ctx)
		{
			ASSERT(false); // must be implemented by derived services
		}
	protected:
		static inline DERIVED* s_instance = nullptr;
		std::string const m_namespace;
	protected:
		service(std::string const& name) :
			m_namespace(name)
		{
			ASSERT(!s_instance);
		}
	protected:
		static void register_function(JSContext* const ctx, std::string const& obj_name, std::string const& fn_name, s32 const num_args, JSCFunction* const fn)
		{
			JSValue const global_obj = JS_GetGlobalObject(ctx);
			JSValue djinn_obj = JS_GetPropertyStr(ctx, global_obj, "djinn");
			if (JS_IsUndefined(djinn_obj))
			{
				djinn_obj = JS_NewObject(ctx);
				JS_SetPropertyStr(ctx, global_obj, "djinn", djinn_obj);
			}
			else
			{
				JS_FreeValue(ctx, djinn_obj);
			}
			JSValue service_obj = JS_GetPropertyStr(ctx, djinn_obj, s_instance->m_namespace.c_str());
			if (JS_IsUndefined(service_obj))
			{
				service_obj = JS_NewObject(ctx);
				JS_SetPropertyStr(ctx, djinn_obj, s_instance->m_namespace.c_str(), service_obj);
			}
			else
			{
				JS_FreeValue(ctx, service_obj);
			}
			JSValue container_obj = obj_name.empty() ? service_obj : JS_GetPropertyStr(ctx, service_obj, obj_name.c_str());
			if (JS_IsUndefined(container_obj))
			{
				container_obj = JS_NewObject(ctx);
				JS_SetPropertyStr(ctx, service_obj, obj_name.c_str(), container_obj);
			}
			else if (!obj_name.empty())
			{
				JS_FreeValue(ctx, container_obj);
			}
			JSValue fn_obj = JS_GetPropertyStr(ctx, container_obj, fn_name.c_str());
			if (JS_IsUndefined(fn_obj))
			{
				fn_obj = JS_NewCFunction(ctx, fn, fn_name.c_str(), num_args);
				JS_SetPropertyStr(ctx, container_obj, fn_name.c_str(), fn_obj);
			}
			else
			{
				ASSERT(false);
			}
			JS_FreeValue(ctx, global_obj);
		}
		static void register_function(JSContext* const ctx, std::string const& fn_name, s32 const num_args, JSCFunction* const fn)
		{
			register_function(ctx, "", fn_name, num_args, fn);
		}
	};
} // namespace djinn
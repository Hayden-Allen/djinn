#include "pch.h"
#include "js.h"

namespace djinn::js
{
	JSValue extract_array_element(JSContext* const ctx, JSValue const& arr, u32 const index)
	{
		return JS_GetPropertyUint32(ctx, arr, index);
	}
	s64 extract_array_length(JSContext* const ctx, JSValue const& val)
	{
		s64 length;
		s32 const ret = JS_GetPropertyLength(ctx, &length, val);
		ASSERT(!ret)
		return length;
	}
	std::vector<u8> extract_u8_array(JSContext* const ctx, JSValue const& val)
	{
		return helper::extract_array<u8, u32>(ctx, val, JS_ToUint32);
	}
	std::vector<u32> extract_u32_array(JSContext* const ctx, JSValue const& val)
	{
		return helper::extract_array<u32, u32>(ctx, val, JS_ToUint32);
	}
	std::vector<id_t> extract_id_array(JSContext* const ctx, JSValue const& val)
	{
		return helper::extract_array<id_t, id_t>(ctx, val, JS_ToUint32);
	}
	std::vector<f32> extract_f32_array(JSContext* const ctx, JSValue const& val)
	{
		return helper::extract_array<f32, f64>(ctx, val, JS_ToFloat64);
	}
	std::vector<std::string> extract_string_array(JSContext* const ctx, JSValue const& val)
	{
		s64 const length = extract_array_length(ctx, val);
		std::vector<std::string> ret(length);
		for (u32 i = 0; i < (u32)length; i++)
		{
			JSValueConst element = JS_GetPropertyUint32(ctx, val, i);
			ret[i] = JS_ToCString(ctx, element);
			JS_FreeValue(ctx, element);
		}
		return ret;
	}
	id_t extract_id(JSContext* const ctx, JSValue const& val)
	{
		return extract_u32(ctx, val); // factor this out so we can change it easier
	}
	JSValue create_id(JSContext* const ctx, u32 const id)
	{
		return JS_NewUint32(ctx, id); // factor this out so we can change it easier
	}
	JSValue create_bool(JSContext* const ctx, bool const b)
	{
		return JS_NewBool(ctx, (s32)b);
	}
	JSValue create_u32(JSContext* const ctx, u32 const u)
	{
		return JS_NewUint32(ctx, u);
	}
	JSValue create_f32(JSContext* const ctx, f32 const f)
	{
		return JS_NewFloat64(ctx, f);
	}
	JSValue create_f32_array(JSContext* const ctx, s64 const count, f32 const* const f)
	{
		JSValue arr = JS_NewArray(ctx);
		for (s64 i = 0; i < count; i++)
			JS_SetPropertyInt64(ctx, arr, i, create_f32(ctx, f[i]));
		return arr;
	}
	s32 extract_s32(JSContext* const ctx, JSValue const& val)
	{
		return helper::extract<s32, s32>(ctx, val, JS_ToInt32);
	}
	u32 extract_u32(JSContext* const ctx, JSValue const& val)
	{
		return helper::extract<u32, u32>(ctx, val, JS_ToUint32);
	}
	f32 extract_f32(JSContext* const ctx, JSValue const& val)
	{
		return helper::extract<f32, f64>(ctx, val, JS_ToFloat64);
	}
	bool extract_bool(JSContext* const ctx, JSValue const& val)
	{
		return JS_ToBool(ctx, val);
	}
	std::string extract_string(JSContext* const ctx, JSValue const& val)
	{
		char const* s = JS_ToCString(ctx, val);
		std::string out = s;
		JS_FreeCString(ctx, s);
		return out;
	}
	std::unordered_map<std::string, JSValue> extract_map(JSContext* const ctx, JSValue const& val)
	{
		std::unordered_map<std::string, JSValue> result;
		JSPropertyEnum* prop_enum;
		u32 num_props;
		JS_GetOwnPropertyNames(ctx, &prop_enum, &num_props, val, JS_GPN_STRING_MASK | JS_GPN_ENUM_ONLY);
		for (u32 i = 0; i < num_props; i++)
		{
			JSValue prop_name = JS_AtomToString(ctx, prop_enum[i].atom);
			char const* prop_name_str = JS_ToCString(ctx, prop_name);
			result[prop_name_str] = JS_GetProperty(ctx, val, prop_enum[i].atom);
			JS_FreeValue(ctx, result[prop_name_str]);
			JS_FreeCString(ctx, prop_name_str);
			JS_FreeValue(ctx, prop_name);
		}
		js_free(ctx, prop_enum);
		return result;
	}
} // namespace djinn::js



namespace djinn::js::global
{
	void init_globals(JSContext* const ctx)
	{
		JSValue const global = JS_GetGlobalObject(ctx);
		JSValue const console = JS_NewObject(ctx);
		JSValue const log = JS_NewCFunction(ctx, console_log, "log", 1);
		JS_SetPropertyStr(ctx, global, "console", console);
		JS_SetPropertyStr(ctx, console, "log", log);

		JS_FreeValue(ctx, global);
	}
	JSValue console_log(JSContext* const ctx, JSValueConst this_val, int const argc, JSValueConst* const argv)
	{
		std::cout << "console.log: ";
		for (int i = 0; i < argc; ++i)
		{
			char const* s = JS_ToCString(ctx, argv[i]);
			std::cout << s << (i < argc - 1 ? " " : "");
			JS_FreeCString(ctx, s);
		}
		std::cout << "\n";

		return JS_UNDEFINED;
	}
} // namespace djinn::js::global

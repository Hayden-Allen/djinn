#pragma once
#include "pch.h"

namespace djinn::js
{
	namespace helper
	{
		template<typename CPP, typename JS, typename FN>
		std::vector<CPP> extract_array(JSContext* const ctx, JSValue const& val, FN const& fn)
		{
			s64 length;
			if (JS_GetPropertyLength(ctx, &length, val))
			{
				ASSERT(false);
				return {};
			}

			std::vector<CPP> ret(length);
			for (u32 i = 0; i < (u32)length; i++)
			{
				JSValueConst element = JS_GetPropertyUint32(ctx, val, i);
				JS out{};
				fn(ctx, &out, element);
				ret[i] = static_cast<CPP>(out);
				JS_FreeValue(ctx, element);
			}
			return ret;
		}
	}
	extern std::vector<u8> extract_u8_array(JSContext* const ctx, JSValue const& val);
	extern std::vector<u32> extract_u32_array(JSContext* const ctx, JSValue const& val);
	extern std::vector<f32> extract_f32_array(JSContext* const ctx, JSValue const& val);
	extern id_t extract_id(JSContext* const ctx, JSValue const& val);
	extern JSValue create_id(JSContext* const ctx, u32 const id);
	extern u32 extract_u32(JSContext* const ctx, JSValue const& val);
	extern f32 extract_f32(JSContext* const ctx, JSValue const& val);
	extern std::string extract_string(JSContext* const ctx, JSValue const& val);
	extern std::unordered_map<std::string, JSValue> extract_map(JSContext* const ctx, JSValue const& val);

	namespace global
	{
		extern void init_globals(JSContext* const ctx);
		extern JSValue console_log(JSContext* const ctx, JSValueConst this_val, int const argc, JSValueConst* const argv);
	} // namespace global
} // namespace djinn::js

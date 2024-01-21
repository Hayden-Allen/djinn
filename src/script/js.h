#pragma once
#include "pch.h"

namespace djinn::js
{
	extern s64 extract_array_length(JSContext* const ctx, JSValue const& val);
	extern std::vector<u8> extract_u8_array(JSContext* const ctx, JSValue const& val);
	extern std::vector<u32> extract_u32_array(JSContext* const ctx, JSValue const& val);
	extern std::vector<id_t> extract_id_array(JSContext* const ctx, JSValue const& val);
	extern std::vector<f32> extract_f32_array(JSContext* const ctx, JSValue const& val);
	extern std::vector<std::string> extract_string_array(JSContext* const ctx, JSValue const& val);
	extern id_t extract_id(JSContext* const ctx, JSValue const& val);
	extern s32 extract_s32(JSContext* const ctx, JSValue const& val);
	extern u32 extract_u32(JSContext* const ctx, JSValue const& val);
	extern f32 extract_f32(JSContext* const ctx, JSValue const& val);
	extern bool extract_bool(JSContext* const ctx, JSValue const& val);
	extern std::string extract_string(JSContext* const ctx, JSValue const& val);
	extern std::unordered_map<std::string, JSValue> extract_map(JSContext* const ctx, JSValue const& val);
	extern JSValue create_id(JSContext* const ctx, u32 const id);
	extern JSValue create_bool(JSContext* const ctx, bool const b);
	extern JSValue create_u32(JSContext* const ctx, u32 const u);
	extern JSValue create_f32(JSContext* const ctx, f32 const f);
	extern JSValue create_string(JSContext* const ctx, std::string const& s);
	extern JSValue create_f32_array(JSContext* const ctx, s64 const count, f32 const* const f);
	extern JSValue create_id_array(JSContext* const ctx, s64 const count, id_t const* const ids);
	template<typename IT>
	extern JSValue create_string_array(JSContext* const ctx, IT const& begin, IT const& end)
	{
		JSValue arr = JS_NewArray(ctx);
		s64 i = 0;
		for (IT it = begin; it != end; it++)
		{
			JS_SetPropertyInt64(ctx, arr, i, create_string(ctx, *it));
			i++;
		}
		return arr;
	}
	template<typename FN>
	void array_for(JSContext* const ctx, JSValue const& arr, FN const& fn)
	{
		s64 const length = extract_array_length(ctx, arr);
		for (u32 i = 0; i < (u32)length; i++)
		{
			JSValue const& element = JS_GetPropertyUint32(ctx, arr, i);
			fn(element, i);
			JS_FreeValue(ctx, element);
		}
	}
	template<space SPACE>
	vec<SPACE> extract_vec(JSContext* const ctx, JSValue const& val)
	{
		std::vector<f32> const& arr = extract_f32_array(ctx, val);
		ASSERT(arr.size() == 3);
		return vec<SPACE>(arr[0], arr[1], arr[2]);
	}
	template<space SPACE>
	point<SPACE> extract_point(JSContext* const ctx, JSValue const& val)
	{
		std::vector<f32> const& arr = extract_f32_array(ctx, val);
		ASSERT(arr.size() == 3);
		return point<SPACE>(arr[0], arr[1], arr[2]);
	}
	template<space SPACE>
	direction<SPACE> extract_direction(JSContext* const ctx, JSValue const& val)
	{
		std::vector<f32> const& arr = extract_f32_array(ctx, val);
		ASSERT(arr.size() == 3);
		return direction<SPACE>(arr[0], arr[1], arr[2]);
	}



	namespace helper
	{
		template<typename CPP, typename JS, typename FN>
		std::vector<CPP> extract_array(JSContext* const ctx, JSValue const& val, FN const& fn)
		{
			s64 const length = extract_array_length(ctx, val);
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
		template<typename CPP, typename JS, typename FN>
		CPP extract(JSContext* const ctx, JSValue const& val, FN const& fn)
		{
			JS out{};
			auto const ret = fn(ctx, &out, val);
			ASSERT(!ret)
			return (CPP)out;
		}
	} // namespace helper



	namespace global
	{
		extern void init_globals(JSContext* const ctx);
		extern JSValue console_log(JSContext* const ctx, JSValueConst this_val, int const argc, JSValueConst* const argv);
	} // namespace global
} // namespace djinn::js

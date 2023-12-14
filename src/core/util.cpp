#include "util.h"

namespace djinn::u
{
	std::string to_absolute(std::filesystem::path const& fp)
	{
		return std::filesystem::absolute(fp).string();
	}
	std::string to_absolute(std::filesystem::path const& dir, std::string const& file)
	{
		return std::filesystem::absolute(dir / file).string();
	}
	std::vector<u32> extract_u32_array(JSContext* const ctx, JSValue const& val)
	{
		s64 length;
		if (JS_GetPropertyLength(ctx, &length, val))
		{
			ASSERT(false);
			return {};
		}

		std::vector<u32> ret(length);
		for (u32 i = 0; i < (u32)length; i++)
		{
			JSValueConst element = JS_GetPropertyUint32(ctx, val, i);
			JS_ToUint32(ctx, &ret[i], element);
			JS_FreeValue(ctx, element);
		}
		return ret;
	}
	std::vector<f32> extract_f32_array(JSContext* const ctx, JSValue const& val)
	{
		s64 length;
		if (JS_GetPropertyLength(ctx, &length, val))
		{
			ASSERT(false);
			return {};
		}

		std::vector<f32> ret(length);
		for (u32 i = 0; i < (u32)length; i++)
		{
			JSValueConst element = JS_GetPropertyUint32(ctx, val, i);
			f64 out;
			JS_ToFloat64(ctx, &out, element);
			ret[i] = (f32)out;
			JS_FreeValue(ctx, element);
		}
		return ret;
	}
	u32 extract_u32(JSContext* const ctx, JSValue const& val, u32* const out)
	{
		if (JS_ToUint32(ctx, out, val))
		{
			ASSERT(false);
			return false;
		}
		return true;
	}
	std::string extract_string(JSContext* const ctx, JSValue const& val)
	{
		char const* s = JS_ToCString(ctx, val);
		std::string out = s;
		JS_FreeCString(ctx, s);
		return out;
	}
} // namespace djinn::u

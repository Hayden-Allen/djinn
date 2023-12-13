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
	std::vector<u32> extract_uint32_array(JSContext* const ctx, JSValue const& val)
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
} // namespace djinn::u

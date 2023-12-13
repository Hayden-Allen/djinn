#include "pch.h"

namespace djinn::u
{
	extern std::string to_absolute(std::filesystem::path const& fp);
	extern std::string to_absolute(std::filesystem::path const& dir, std::string const& file);
	extern std::vector<u32> extract_uint32_array(JSContext* const ctx, JSValue const& val);
} // namespace djinn::u
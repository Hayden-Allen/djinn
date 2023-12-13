#pragma once 
#include "pch.h"

namespace djinn::u
{
	extern std::string to_absolute(std::filesystem::path const& fp);
	extern std::string to_absolute(std::filesystem::path const& dir, std::string const& file);
	extern std::vector<u32> extract_u32_array(JSContext* const ctx, JSValue const& val);
	extern u32 extract_u32(JSContext* const ctx, JSValue const& val, u32* const out);
} // namespace djinn::u

#pragma once
#include "pch.h"

namespace djinn::u
{
	extern std::string to_absolute(std::filesystem::path const& fp);
	extern std::string to_absolute(std::filesystem::path const& dir, std::string const& file);
	extern u8* load_texture2d_rgba_u8_raw(std::string const& fp, s32* const out_width, s32* const out_height);
	extern texture2d_rgba_u8* load_texture2d_rgba_u8(std::string const& fp, texture_options const& options);
} // namespace djinn::u

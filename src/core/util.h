#pragma once
#include "pch.h"

namespace djinn::u
{
	extern std::string to_absolute(std::filesystem::path const& fp);
	extern std::string to_absolute(std::filesystem::path const& dir, std::string const& file);
	extern u8* load_texture2d_rgba_u8_raw(std::string const& fp, s32* const out_width, s32* const out_height);
	extern texture2d_rgba_u8* load_texture2d_rgba_u8(std::string const& fp, texture_options const& options);
	extern void sleep(u32 const ms);
	extern std::string read_file(std::string const& afp);
	extern std::vector<char> read_file_binary(std::string const& afp);
	extern std::string trim(std::string const& s);
	extern std::vector<std::string> split(std::string const& string, std::string const& delim);
	extern bool iequals(std::string const& a, std::string const& b);
	// https://stackoverflow.com/a/19195373
	template<typename T>
	u64 hash_combine(u64 const in, T const& v)
	{
		return in ^ (std::hash<T>{}(v) + 0x9e3779b9 + (in << 6) + (in >> 2));
	}
} // namespace djinn::u

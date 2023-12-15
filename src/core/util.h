#pragma once
#include "pch.h"

namespace djinn::u
{
	extern std::string to_absolute(std::filesystem::path const& fp);
	extern std::string to_absolute(std::filesystem::path const& dir, std::string const& file);
} // namespace djinn::u

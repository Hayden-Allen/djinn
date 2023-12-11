#include "pch.h"

namespace u
{
	extern std::string to_absolute(std::filesystem::path const& fp);
	extern std::string to_absolute(std::filesystem::path const& dir, std::string const& file);
}
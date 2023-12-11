#include "util.h"

std::string u::to_absolute(std::filesystem::path const& fp)
{
	return std::filesystem::absolute(fp).string();
}
std::string u::to_absolute(std::filesystem::path const& dir, std::string const& file)
{
	return std::filesystem::absolute(dir / file).string();
}

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
	u8* load_texture2d_rgba_u8_raw(std::string const& fp, s32* const out_width, s32* const out_height)
	{
		std::ifstream test(fp);
		if (!test.is_open())
		{
			ASSERT(false)
			return nullptr;
		}
		test.close();

		stbi_set_flip_vertically_on_load(true);
		int w = -1, h = -1, c = -1;
		stbi_uc* const tex_data = stbi_load(fp.c_str(), &w, &h, &c, 4);
		ASSERT(tex_data != nullptr)
		ASSERT(c >= 3)
		if (out_width)
			*out_width = w;
		if (out_height)
			*out_height = h;
		return tex_data;
	}
	texture2d_rgba_u8* load_texture2d_rgba_u8(std::string const& fp, texture_options const& options)
	{
		s32 w, h;
		stbi_uc* const tex_data = load_texture2d_rgba_u8_raw(fp, &w, &h);
		texture2d_rgba_u8* tex = new mgl::texture2d_rgba_u8(GL_RGBA, w, h, tex_data, options);
		stbi_image_free(tex_data);
		return tex;
	}
	retained_texture2d_rgba_u8_array* load_retained_texture2d_rgba_u8_array(std::string const& fp, texture_options const& options)
	{
		stbi_set_flip_vertically_on_load(true);
		std::ifstream test(fp);
		if (!test.is_open())
		{
			ASSERT(false);
			return nullptr;
		}
		test.close();

		mgl::retained_texture_array<mgl::retained_texture2d_rgba_u8>* ret = nullptr;
		if (fp.ends_with(".gif"))
		{
			std::ifstream ifs(fp, std::ios::binary | std::ios::ate);
			const u64 size = ifs.tellg();
			ifs.seekg(0, std::ios::beg);
			std::vector<char> raw(size);
			ifs.read((char*)raw.data(), size);

			s32* delays = nullptr;
			s32 x, y, z, comp;
			stbi_uc* const tex_data = stbi_load_gif_from_memory((stbi_uc*)raw.data(), (s32)raw.size(), &delays, &x, &y, &z, &comp, 4);
			ret = new mgl::retained_texture_array<mgl::retained_texture2d_rgba_u8>(GL_RGBA, x, y, tex_data, z, delays);
			stbi_image_free(tex_data);
			free(delays);
		}
		else
		{
			int w = -1, h = -1, c = -1;
			stbi_uc* const tex_data = stbi_load(fp.c_str(), &w, &h, &c, 4);
			assert(c >= 3);
			ret = new mgl::retained_texture_array<mgl::retained_texture2d_rgba_u8>(GL_RGBA, w, h, tex_data);
			stbi_image_free(tex_data);
		}
		return ret;
	}
	void sleep(u32 const ms)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
	}
	std::string read_file(std::string const& afp)
	{
		std::ifstream ifs(afp);
		ASSERT(ifs.is_open());
		std::ostringstream sstr;
		sstr << ifs.rdbuf();
		return sstr.str();
	}
	std::vector<char> read_file_binary(std::string const& afp)
	{
		std::ifstream ifs(afp, std::ios::binary | std::ios::ate);
		ASSERT(ifs.is_open());
		u64 const size = ifs.tellg();
		ifs.seekg(0, std::ios::beg);
		std::vector<char> ret(size);
		ifs.read((char*)ret.data(), size);
		return ret;
	}
	std::string trim(std::string const& s)
	{
		std::string ret = s;
		ret.erase(0, ret.find_first_not_of(" \n\r\t"));
		ret.erase(ret.find_last_not_of(" \n\r\t") + 1);
		return ret;
	}
	// https://gist.github.com/tcmug/9712f9192571c5fe65c362e6e86266f8
	std::vector<std::string> split(std::string const& string, std::string const& delim)
	{
		std::vector<std::string> result;
		size_t from = 0, to = 0;
		while (std::string::npos != (to = string.find(delim, from)))
		{
			result.push_back(string.substr(from, to - from));
			from = to + delim.length();
		}
		result.push_back(string.substr(from, to));
		return result;
	}
	bool iequals(std::string const& a, std::string const& b)
	{
		return (a.size() == b.size()) &&
			   std::equal(a.begin(), a.end(), b.begin(), [](char const ca, char const cb)
				   {
					   return std::tolower(ca) == std::tolower(cb);
				   });
	}
} // namespace djinn::u

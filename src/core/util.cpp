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
			ASSERT(false);
			return nullptr;
		}
		test.close();

		stbi_set_flip_vertically_on_load(true);
		int w = -1, h = -1, c = -1;
		stbi_uc* const tex_data = stbi_load(fp.c_str(), &w, &h, &c, 4);
		ASSERT(c >= 3);
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
} // namespace djinn::u

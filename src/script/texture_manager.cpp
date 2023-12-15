#include "pch.h"
#include "texture_manager.h"
#include "core/constants.h"

namespace djinn
{
	texture_manager::texture_manager() :
		manager(c::base_dir::texture)
	{}



	id_t texture_manager::create(u32 const width, u32 const height, texture_options const& options)
	{
		texture2d_rgba_u8* const tex = new texture2d_rgba_u8(width, height, options);
		return insert(tex);
	}
	id_t texture_manager::load(std::string const& fp)
	{
		ASSERT(false);
		return 0;
	}
	void texture_manager::destroy(id_t const id)
	{
		erase(id);
	}
	void texture_manager::reload(std::string const& fp)
	{
		ASSERT(false);
	}
	void texture_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		ASSERT(false);
	}
	void texture_manager::update(id_t const id, std::vector<u8> const& subpixels, texture_options const& options)
	{
		sptr<texture2d_rgba_u8> tex = get(id);
		tex->init(GL_RGBA, tex->get_width(), tex->get_height(), subpixels.data(), options);
	}
	void texture_manager::bind(id_t const id, u32 const slot)
	{
		sptr<texture2d_rgba_u8> tex = get(id);
		tex->bind(slot);
	}
} // namespace djinn
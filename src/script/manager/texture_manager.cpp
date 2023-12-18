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
		id_t const id = insert(tex);
		m_id2options.insert({ id, options });
		return id;
	}
	id_t texture_manager::load(std::string const& fp)
	{
		ASSERT(false);
		return 0;
	}
	id_t texture_manager::load(std::string const& fp, texture_options const& options)
	{
		std::string const& afp = to_absolute(fp);
		texture2d_rgba_u8* const tex = u::load_texture2d_rgba_u8(afp, options);
		id_t const id = insert(tex);
		m_id2afp.insert(id, afp);
		m_id2options.insert({ id, options });
		return id;
	}
	void texture_manager::destroy(id_t const id)
	{
		m_id2afp.erase_key(id);
		m_id2options.erase(id);
		erase(id);
	}
	void texture_manager::reload(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		if (!m_id2afp.contains_val(afp)) // either this texture is not loaded or it is not a texture2d
			return;
		s32 width, height;
		u8* const data = u::load_texture2d_rgba_u8_raw(afp, &width, &height);

		auto const& ids = m_id2afp.get_key(afp);
		for (id_t const id : ids)
		{
			get(id)->init(GL_RGBA, width, height, data, m_id2options.at(id));
		}

		stbi_image_free(data);
	}
	void texture_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		std::string const& old_afp = to_absolute(old_fp);
		if (!m_id2afp.contains_val(old_afp)) // either this texture is not loaded or it is not a texture2d
			return;
		std::string const& new_afp = to_absolute(new_fp);
		m_id2afp.replace_val(old_afp, new_afp);
	}
	void texture_manager::update(id_t const id, std::vector<u8> const& subpixels, texture_options const& options)
	{
		sptr<texture2d_rgba_u8> tex = get(id);
		tex->init(GL_RGBA, tex->get_width(), tex->get_height(), subpixels.data(), options);
	}
	void texture_manager::update(id_t const id, std::vector<u8> const& subpixels)
	{
		update(id, subpixels, m_id2options.at(id));
	}
	void texture_manager::bind(id_t const id, u32 const slot)
	{
		sptr<texture2d_rgba_u8> tex = get(id);
		tex->bind(slot);
	}
} // namespace djinn
#include "pch.h"
#include "texture_manager.h"
#include "core/constants.h"
#include "script/service/asset_service.h"

namespace djinn
{
	texture_manager::texture_manager() :
		ref_counted_file_manager(c::base_dir::texture)
	{}



	void texture_manager::update_all(f32 const milliseconds)
	{
		for (auto& pair : m_objects)
		{
			pair.second->update((u64)milliseconds);
		}
	}
	id_t texture_manager::create(u32 const width, u32 const height, texture_options const& options)
	{
		/*retained_texture2d_rgba_u8_array* const tex = new retained_texture2d_rgba_u8_array(width, height, options);
		id_t const id = insert(tex);
		m_id2options.insert({ id, options });
		return id;*/
		ASSERT(false);
		return 0;
	}
	std::pair<std::vector<id_t>, std::vector<sptr<retained_texture2d_rgba_u8_array>>> texture_manager::load_xport(haul::input_file* const in)
	{
		u64 const count = in->get64();
		printf("t: %zu\n", count);
		std::vector<id_t> ids;
		std::vector<sptr<retained_texture2d_rgba_u8_array>> tex;
		ids.reserve(count);
		tex.reserve(count);
		for (u64 i = 0; i < count; i++)
		{
			retained_texture2d_rgba_u8_array* const t = new retained_texture2d_rgba_u8_array(in, asset_service::get_default_texture_options());
			id_t const id = insert(t);
			ids.emplace_back(id);
			tex.emplace_back(get(id));
		}
		return { ids, tex };
	}
	id_t texture_manager::load(std::string const& fp)
	{
		ASSERT(false)
		return 0;
	}
	id_t texture_manager::load(std::string const& fp, texture_options const& options)
	{
		std::string const& afp = to_absolute(fp);
		id_t const existing_id = find_existing(afp, options);
		// the texture at the given filepath has already been created (with given options)
		if (existing_id != 0)
		{
			m_id2ref[existing_id]++;
			return existing_id;
		}
		// no such texture exists, create it
		retained_texture2d_rgba_u8_array* const tex = u::load_retained_texture2d_rgba_u8_array(afp, options);
		id_t const id = insert(tex);
		m_id2afp.insert(id, afp);
		m_id2options.insert({ id, options });
		return id;
	}
	void texture_manager::destroy(id_t const id)
	{
		if (try_erase(id))
		{
			if (m_id2afp.contains_key(id))
			{
				m_id2afp.erase_key(id);
				m_id2options.erase(id);
			}
		}
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
			get<texture2d_rgba_u8>(id)->init(GL_RGBA, width, height, data, m_id2options.at(id));
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
		/*auto tex = get(id);
		tex->init(GL_RGBA, tex->get_width(), tex->get_height(), subpixels.data(), options);*/
		ASSERT(false);
	}
	void texture_manager::update(id_t const id, std::vector<u8> const& subpixels)
	{
		update(id, subpixels, m_id2options.at(id));
	}
	void texture_manager::bind(id_t const id, u32 const slot)
	{
		get<texture2d_rgba_u8>(id)->bind(slot);
	}



	id_t texture_manager::find_existing(std::string const& afp, texture_options const& options)
	{
		if (!m_id2afp.contains_val(afp))
			return 0;
		std::unordered_set<id_t> const& ids = m_id2afp.get_key(afp);
		for (id_t const id : ids)
		{
			texture_options const& opts = m_id2options.at(id);
			if (options == opts)
				return id;
		}
		return 0;
	}
} // namespace djinn
#include "pch.h"
#include "cubemap_manager.h"
#include "core/constants.h"

namespace djinn
{
	cubemap_manager::cubemap_manager() :
		manager(c::base_dir::texture)
	{}
	cubemap_manager::~cubemap_manager()
	{
		for (auto& pair : m_afp2pixels)
			stbi_image_free(pair.second);
	}



	id_t cubemap_manager::create(u32 const width, u32 const height, texture_options const& options)
	{
		cubemap_rgba_u8* const tex = new cubemap_rgba_u8(width, height, options);
		id_t const id = insert(tex);
		m_id2options.insert({ id, options });
		return id;
	}
	id_t cubemap_manager::load(std::string const& fp)
	{
		ASSERT(false);
		return 0;
	}
	id_t cubemap_manager::load(std::array<std::string, 6> const& fps, texture_options const& options)
	{
		s32 width, height;
		std::array<u8*, 6> const& pixels = load(fps, &width, &height);
		cubemap_rgba_u8* const tex = new cubemap_rgba_u8(GL_RGBA, width, height, pixels.data(), options);
		id_t const id = insert(tex);
		m_id2options.insert({ id, options });

		for (s32 i = 0; i < 6; i++)
		{
			std::string const& afp = to_absolute(fps[i]);
			m_id2afps[id][i] = afp;
			m_afp2ids[afp].insert(id);
		}

		return id;
	}
	void cubemap_manager::destroy(id_t const id)
	{
		auto const& it = m_id2afps.find(id);
		if (it != m_id2afps.end())
		{
			std::array<std::string, 6> const& afps = it->second;
			for (std::string const& afp : afps)
			{
				std::unordered_set<id_t>& ids = m_afp2ids.at(afp);
				ids.erase(id);
				if (!ids.size())
				{
					m_afp2ids.erase(afp);
					stbi_image_free(m_afp2pixels.at(afp));
					m_afp2pixels.erase(afp);
				}
			}
			m_id2afps.erase(id);
		}
		erase(id);
	}
	void cubemap_manager::reload(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		if (!m_afp2ids.contains(afp)) // either this texture is not loaded or it is not part of a cubemap
			return;
		std::unordered_set<id_t> const& ids = m_afp2ids.at(afp);
		for (id_t const id : ids)
		{
			s32 new_width, new_height;
			std::array<std::string, 6> const& existing_afps = m_id2afps.at(id);
			std::array<u8*, 6> pixels = { nullptr };
			for (s32 i = 0; i < 6; i++)
			{
				std::string const& existing_afp = existing_afps.at(i);
				if (existing_afp == afp)
				{
					u8* const new_pixels = load_one(afp, &new_width, &new_height);
					m_afp2pixels[afp] = pixels[i] = new_pixels;
				}
				else
					pixels[i] = m_afp2pixels.at(existing_afp);
			}
			sptr<cubemap_rgba_u8> tex = get(id);
			if (new_width != tex->get_width() || new_height != tex->get_height())
				ASSERT(false)
			tex->init(GL_RGBA, new_width, new_height, pixels.data(), m_id2options.at(id));
		}
	}
	void cubemap_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		std::string const& old_afp = to_absolute(old_fp);
		if (!m_afp2ids.contains(old_afp)) // either this texture is not loaded or it is not part of a cubemap
			return;
		std::string const& new_afp = to_absolute(new_fp);
		std::unordered_set<id_t> const& ids = m_afp2ids.at(old_afp);
		for (id_t const id : ids)
		{
			std::array<std::string, 6>& afps = m_id2afps.at(id);
			for (s32 i = 0; i < 6; i++)
				if (afps[i] == old_afp)
					afps[i] = new_afp;
		}
		auto entry = m_afp2ids.extract(old_afp);
		entry.key() = new_afp;
		m_afp2ids.insert(std::move(entry));
		m_afp2pixels.insert({ new_afp, m_afp2pixels.at(old_afp) });
		m_afp2pixels.erase(old_afp);
	}
	void cubemap_manager::update(id_t const id, std::array<std::vector<u8>, 6> const& subpixels, texture_options const& options)
	{
		u8 const* raw_subpixels[6] = { nullptr };
		for (s32 i = 0; i < 6; i++)
			raw_subpixels[i] = subpixels[i].data();
		sptr<cubemap_rgba_u8> tex = get(id);
		tex->init(GL_RGBA, tex->get_width(), tex->get_height(), raw_subpixels, options);
	}
	void cubemap_manager::update(id_t const id, std::array<std::vector<u8>, 6> const& subpixels)
	{
		update(id, subpixels, m_id2options.at(id));
	}
	void cubemap_manager::bind(id_t const id, u32 const slot)
	{
		sptr<cubemap_rgba_u8> tex = get(id);
		tex->bind(slot);
	}
} // namespace djinn

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
		for (auto& pair : m_fp2pixels)
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
			m_id2fps[id][i] = afp;
			m_fp2ids[afp].insert(id);
		}

		return id;
	}
	void cubemap_manager::destroy(id_t const id)
	{
		auto const& it = m_id2fps.find(id);
		if (it != m_id2fps.end())
		{
			std::array<std::string, 6> const& afps = it->second;
			for (std::string const& afp : afps)
			{
				std::unordered_set<id_t>& ids = m_fp2ids.at(afp);
				ids.erase(id);
				if (!ids.size())
					m_fp2ids.erase(afp);
			}
			m_id2fps.erase(id);
		}
		erase(id);
	}
	void cubemap_manager::reload(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		if (!m_fp2ids.contains(afp)) // either this texture is not loaded or it is not part of a cubemap
			return;
		std::unordered_set<id_t> const& ids = m_fp2ids.at(afp);
		for (id_t const id : ids)
		{
			s32 width, height;
			std::array<std::string, 6> const& existing_afps = m_id2fps.at(id);
			std::array<u8*, 6> pixels = { nullptr };
			for (s32 i = 0; i < 6; i++)
			{
				std::string const& existing_afp = existing_afps.at(i);
				if (existing_afp == afp)
				{
					u8* const new_pixels = load_one(afp, &width, &height);
					m_fp2pixels[afp] = pixels[i] = new_pixels;
				}
				else
					pixels[i] = m_fp2pixels.at(afp);
			}
			get(id)->init(GL_RGBA, width, height, pixels.data(), m_id2options.at(id));
		}
	}
	void cubemap_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		std::string const& old_afp = to_absolute(old_fp);
		ASSERT(m_fp2ids.contains(old_afp));
		std::string const& new_afp = to_absolute(new_fp);
		std::unordered_set<id_t> const& ids = m_fp2ids.at(old_afp);
		for (id_t const id : ids)
		{
			std::array<std::string, 6>& fps = m_id2fps.at(id);
			for (s32 i = 0; i < 6; i++)
				if (fps[i] == old_afp)
					fps[i] = new_afp;
		}
		auto entry = m_fp2ids.extract(old_afp);
		entry.key() = new_afp;
		m_fp2ids.insert(std::move(entry));
	}
	void cubemap_manager::update(id_t const id, std::vector<u8> const& subpixels, texture_options const& options)
	{
		ASSERT(false);
	}
	void cubemap_manager::bind(id_t const id, u32 const slot)
	{
		sptr<cubemap_rgba_u8> tex = get(id);
		tex->bind(slot);
	}
} // namespace djinn

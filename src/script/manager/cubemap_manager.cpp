#include "pch.h"
#include "cubemap_manager.h"
#include "core/constants.h"

namespace djinn
{
	cubemap_manager::cubemap_manager() :
		manager(c::base_dir::texture)
	{}



	id_t cubemap_manager::create(u32 const width, u32 const height, texture_options const& options)
	{
		cubemap_rgba_u8* const tex = new cubemap_rgba_u8(width, height, options);
		id_t const id = insert(tex);
		m_id2options.insert({ id, options });
		return id;
	}
	id_t cubemap_manager::load(std::string const& fp)
	{
		return id_t();
	}
	id_t cubemap_manager::load(std::vector<std::string> const& fps, texture_options const& options)
	{
		s32 width, height;
		std::array<u8*, 6> const& pixels = load(fps, &width, &height);
		cubemap_rgba_u8* const tex = new cubemap_rgba_u8(GL_RGBA, width, height, pixels.data(), options);
		id_t const id = insert(tex);
		m_id2options.insert({ id, options });

		for (std::string const& fp : fps)
		{
			m_id2fps[id].insert(fp);
			m_fp2ids[fp].insert(id);
		}

		for (s32 i = 0; i < 6; i++)
			stbi_image_free(pixels[i]);

		return id;
	}
	void cubemap_manager::destroy(id_t const id)
	{
		auto const& it = m_id2fps.find(id);
		if (it != m_id2fps.end())
		{
			std::unordered_set<std::string> const& fps = it->second;
			for (std::string const& fp : fps)
				m_fp2ids.at(fp).erase(id);
			m_id2fps.erase(id);
		}
		erase(id);
	}
	void cubemap_manager::reload(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		ASSERT(m_fp2ids.contains(fp));
		std::unordered_set<id_t> const& ids = m_fp2ids.at(fp);
		for (id_t const id : ids)
		{
			s32 width, height;
			std::array<u8*, 6> const& pixels = load(m_id2fps.at(id), &width, &height);
			get(id)->init(GL_RGBA, width, height, pixels.data(), m_id2options.at(id));
			for (s32 i = 0; i < 6; i++)
				stbi_image_free(pixels[i]);
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
			std::unordered_set<std::string>& fps = m_id2fps.at(id);
			fps.erase(old_afp);
			fps.insert(new_afp);
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

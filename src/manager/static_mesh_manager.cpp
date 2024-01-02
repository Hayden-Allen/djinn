#include "pch.h"
#include "static_mesh_manager.h"
#include "core/constants.h"
#include "core/util.h"

namespace djinn
{
	static_mesh_manager::static_mesh_manager() :
		ref_counted_file_manager(c::base_dir::mesh)
	{}
	static_mesh_manager::~static_mesh_manager()
	{
		for (auto& pair : m_afp2raw)
			delete pair.second;
	}



	id_t static_mesh_manager::load(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		m3d_t const* const raw = get_or_load(afp);
		static_mesh* const mesh = new static_mesh(raw);
		id_t const id = insert(mesh);
		m_id2afp.insert(id, afp);
		return id;
	}
	void static_mesh_manager::reload(std::string const& fp)
	{
		std::string const& afp = u::to_absolute(fp);
		if (!m_id2afp.contains_val(afp))
			return;

		ASSERT(m_afp2raw.contains(afp));
		delete m_afp2raw.at(afp);

		m3d_t* const new_mesh = load_raw(afp);
		m_afp2raw.insert({ afp, new_mesh });
		for (id_t const id : m_id2afp.get_key(afp))
		{
			get(id)->init(new_mesh);
		}
	}
	void static_mesh_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		std::string const& old_afp = u::to_absolute(old_fp);
		std::string const& new_afp = u::to_absolute(new_fp);
		m_id2afp.replace_val(old_afp, new_afp);
		ASSERT(m_afp2raw.contains(old_afp));
		auto entry = m_afp2raw.extract(old_afp);
		entry.key() = new_afp;
		m_afp2raw.insert(std::move(entry));
	}
	void static_mesh_manager::destroy(id_t const id)
	{
		if (try_erase(id))
		{
			m_id2afp.erase_key(id);
		}
	}



	m3d_t* static_mesh_manager::get_or_load(std::string const& afp)
	{
		auto const& it = m_afp2raw.find(afp);
		if (it != m_afp2raw.end())
			return it->second;
		return load_raw(afp);
	}
	m3d_t* static_mesh_manager::load_raw(std::string const& afp)
	{
		ASSERT(!m_afp2raw.contains(afp));
		std::vector<char> data = u::read_file_binary(afp);
		m3d_t* const raw = m3d_load((u8*)data.data(), nullptr, nullptr, nullptr);
		m_afp2raw.insert({ afp, raw });
		return raw;
	}
} // namespace djinn

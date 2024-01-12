#pragma once
#include "pch.h"
#include "manager/manager.h"
#include "asset/mesh/static_mesh.h"
#include "asset/mesh/animated_mesh.h"

namespace djinn
{
	template<typename T>
	class loaded_mesh_manager : public ref_counted_file_manager<T>
	{
	public:
		DCM(loaded_mesh_manager);
		~loaded_mesh_manager()
		{
			for (auto& pair : m_afp2raw)
			{
				for (u32 i = 0; i < pair.second->numaction; i++)
					delete[] pair.second->action[i].name;
				delete pair.second;
			}
		}
	public:
		id_t load(std::string const& fp) override final
		{
			std::string const& afp = this->to_absolute(fp);
			m3d_t* const raw = get_or_load(afp);
			T* const mesh = new T(raw);
			id_t const id = this->insert(mesh);
			m_id2afp.insert(id, afp);
			return id;
		}
		void reload(std::string const& fp) override final
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
				this->get(id)->init(new_mesh);
			}
		}
		void rename(std::string const& old_fp, std::string const& new_fp) override final
		{
			std::string const& old_afp = u::to_absolute(old_fp);
			std::string const& new_afp = u::to_absolute(new_fp);
			m_id2afp.replace_val(old_afp, new_afp);
			ASSERT(m_afp2raw.contains(old_afp));
			auto entry = m_afp2raw.extract(old_afp);
			entry.key() = new_afp;
			m_afp2raw.insert(std::move(entry));
		}
		void destroy(id_t const id) override final
		{
			if (this->try_erase(id))
			{
				m_id2afp.erase_key(id);
			}
		}
	protected:
		haul::bdumap<id_t, std::string> m_id2afp;
		std::unordered_map<std::string, m3d_t*> m_afp2raw;
	protected:
		loaded_mesh_manager() :
			ref_counted_file_manager<T>(c::base_dir::mesh)
		{}
	protected:
		m3d_t* get_or_load(std::string const& afp)
		{
			auto const& it = m_afp2raw.find(afp);
			if (it != m_afp2raw.end())
				return it->second;
			return load_raw(afp);
		}
		m3d_t* load_raw(std::string const& afp)
		{
			ASSERT(!m_afp2raw.contains(afp));
			std::vector<char> data = u::read_file_binary(afp);
			m3d_t* const raw = m3d_load((u8*)data.data(), nullptr, nullptr, nullptr);
			for (u32 i = 0; i < raw->numaction; i++)
			{
				std::string backup(raw->action[i].name);
				raw->action[i].name = new char[backup.size() + 1];
				std::strcpy(raw->action[i].name, backup.data());
			}
			m_afp2raw.insert({ afp, raw });
			return raw;
		}
	};



	class static_mesh_manager final : public loaded_mesh_manager<static_mesh>
	{};



	class animated_mesh_manager final : public loaded_mesh_manager<animated_mesh>
	{};
} // namespace djinn

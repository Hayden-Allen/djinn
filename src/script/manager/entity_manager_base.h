#pragma once
#include "pch.h"
#include "manager.h"
#include "core/constants.h"

namespace djinn
{
	template<typename T>
	class entity_manager_base : public manager<T>
	{
	public:
		entity_manager_base(JSRuntime* const runtime) :
			manager<T>(c::base_dir::script),
			m_runtime(runtime)
		{}
		DCM(entity_manager_base);
	public:
		virtual id_t load(std::string const& fp) override = 0;
		void reload(std::string const& fp) override final
		{
			std::string const& afp = this->to_absolute(fp);
			if (!m_afp2src.contains(afp))
				return;
			load_file(afp);
		}
		void rename(std::string const& old_fp, std::string const& new_fp) override final
		{
			std::string const& old_afp = this->to_absolute(old_fp);
			if (!m_afp2src.contains(old_afp))
				return;
			std::string const& new_afp = this->to_absolute(new_fp);
			m_afp2src.insert({ new_afp, m_afp2src.at(old_afp) });
			m_afp2src.erase(old_afp);
			m_id2afp.replace_val(old_afp, new_afp);
		}
		void destroy(id_t const id) override final
		{
			if (this->try_erase(id))
			{
				m_id2afp.erase_key(id);
			}
		}
	protected:
		JSRuntime* const m_runtime;
		std::unordered_map<std::string, std::string> m_afp2src;
		haul::bdumap<id_t, std::string> m_id2afp;
	protected:
		void inject_script(std::string const& afp)
		{
			std::string const& src = m_afp2src.at(afp);
			for (id_t const id : m_id2afp.get_key(afp))
			{
				sptr<T> ptr = this->get(id);
				ptr->inject_script(afp, src);
			}
		}
		void load_file(std::string const& afp)
		{
			m_afp2src.insert_or_assign(afp, u::read_file(afp));
			inject_script(afp);
		}
		id_t load_base(T* const t, std::string const& afp)
		{
			id_t const id = this->insert(t);
			m_id2afp.insert(id, afp);
			if (m_afp2src.contains(afp))
				// inject_script(afp);
				t->inject_script(afp, m_afp2src.at(afp));
			else
				load_file(afp);
			return id;
		}
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "manager/manager.h"
#include "core/constants.h"

namespace djinn
{
	template<typename T>
	class entity_manager_base : public file_manager<T>
	{
	public:
		entity_manager_base(JSRuntime* const runtime) :
			file_manager<T>(c::base_dir::script),
			m_runtime(runtime)
		{}
		DCM(entity_manager_base);
	public:
		id_t load(std::string const& fp) override final
		{
			ASSERT(false);
			return 0;
		}
		virtual id_t load(std::string const& fp, s32 const argc, JSValueConst* const argv) = 0;
		void reload(std::string const& fp) override final
		{
			std::string const& afp = this->to_absolute(fp);
			if (!m_afp2src.contains(afp))
				return;
			load_file(afp, 0, nullptr);
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
		void inject_script(std::string const& afp, s32 const argc, JSValueConst* const argv)
		{
			std::string const& src = m_afp2src.at(afp);
			for (id_t const id : m_id2afp.get_key(afp))
			{
				sptr<T> ptr = this->get(id);
				ptr->inject_script(afp, src, argc, argv);
			}
		}
		void load_file(std::string const& afp, s32 const argc, JSValueConst* const argv)
		{
			std::string const& src = u::read_file(afp);
			if (src.empty()) return;
			m_afp2src.insert_or_assign(afp, src);
			inject_script(afp, argc, argv);
		}
		id_t load_base(T* const t, std::string const& afp, s32 const argc, JSValueConst* const argv)
		{
			id_t const id = this->insert(t);
			m_id2afp.insert(id, afp);
			if (m_afp2src.contains(afp))
				t->inject_script(afp, m_afp2src.at(afp), argc, argv);
			else
				load_file(afp, argc, argv);
			return id;
		}
	};
} // namespace djinn

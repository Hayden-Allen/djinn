#pragma once
#include "pch.h"
#include "core/util.h"

namespace djinn
{
	template<typename T>
	class manager
	{
	public:
		manager(std::string const& base_dir) :
			m_base_dir(base_dir)
		{}
		DCM(manager);
		virtual ~manager()
		{
			for (auto& pair : m_objects)
				pair.second.free();
		}
	public:
		virtual id_t load(std::string const& fp) = 0;
		virtual void remove(id_t const id) = 0;
		virtual void reload(std::string const& fp) = 0;
		virtual void rename(std::string const& old_fp, std::string const& new_fp) = 0;
	protected:
		static inline id_t s_next_id = 1;
	protected:
		std::string m_base_dir;	// directory that is watched for updates
		std::unordered_map<id_t, optr<T>> m_objects;	// map of unique id to actual object (this class owns all the objects)
	protected:
		std::string to_absolute(std::string const& fp)
		{
			return u::to_absolute(m_base_dir, fp);
		}
		id_t insert(T* const t)
		{
			id_t const id = s_next_id;
			s_next_id++;
			m_objects.insert({ id, optr<T>(t) });
			return id;
		}
		void erase(id_t const id)
		{
			ASSERT(m_objects.contains(id));
			optr<T>& ptr = m_objects.at(id);
			ptr.free();
			m_objects.erase(id);
		}
	};
} // namespace djinn

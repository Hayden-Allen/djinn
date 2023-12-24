#pragma once
#include "pch.h"
#include "core/util.h"

namespace djinn
{
	class manager_counter
	{
	public:
		DCM(manager_counter);
	protected:
		static inline id_t s_next_id = 1;
	protected:
		manager_counter() {}
	};



	template<typename T>
	class manager : public manager_counter
	{
	public:
		manager(std::string const& base_dir) :
			m_base_dir(base_dir)
		{}
		DCM(manager);
		virtual ~manager()
		{
			for (auto& pair : m_objects)
			{
				pair.second.free();
			}
		}
	public:
		virtual id_t load(std::string const& fp) = 0;
		virtual void reload(std::string const& fp) = 0;
		virtual void rename(std::string const& old_fp, std::string const& new_fp) = 0;
		virtual void destroy(id_t const id) = 0;
		sptr<T> get(id_t const id)
		{
			ASSERT(m_objects.contains(id))
			return m_objects.at(id);
		}
		bool has(id_t const id) const
		{
			return m_objects.contains(id);
		}
		template<typename FN>
		void for_each(FN const& fn)
		{
			for (auto& pair : m_objects)
				fn(sptr<T>(pair.second), pair.first);
		}
		template<typename FN>
		void for_each(FN const& fn) const
		{
			for (auto const& pair : m_objects)
				fn(sptr<T>(pair.second), pair.first);
		}
	protected:
		std::string m_base_dir;
		std::unordered_map<id_t, optr<T>> m_objects; // map of unique id to actual object (this class owns all the objects)
	protected:
		std::string to_absolute(std::string const& fp)
		{
			return u::to_absolute(m_base_dir, fp);
		}
		virtual id_t insert(T* const t)
		{
			id_t const id = s_next_id;
			s_next_id++;
			m_objects.insert({ id, optr<T>(t) });
			return id;
		}
		virtual bool try_erase(id_t const id)
		{
			optr<T>& ptr = m_objects.at(id);
			ptr.free();
			m_objects.erase(id);
			return true;
		}
	};



	template<typename T>
	class ref_counted_manager : public manager<T>
	{
	public:
		ref_counted_manager(std::string const& base_dir) :
			manager<T>(base_dir)
		{}
		DCM(ref_counted_manager);
		virtual ~ref_counted_manager()
		{
			ASSERT(m_id2ref.empty());
		}
	protected:
		std::unordered_map<id_t, u64> m_id2ref; // map unique id to its ref count
	protected:
		id_t insert(T* const t) override final
		{
			id_t const id = manager<T>::insert(t);
			m_id2ref.insert({ id, 1 });
			return id;
		}
		bool try_erase(id_t const id) override final
		{
			ASSERT(m_id2ref.contains(id));
			m_id2ref[id]--;
			if (m_id2ref.at(id) == 0)
			{
				manager<T>::try_erase(id);
				m_id2ref.erase(id);
				return true;
			}
			return false;
		}
	};
} // namespace djinn

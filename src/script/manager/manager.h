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
		manager() {}
		DCM(manager);
		virtual ~manager()
		{
			free_all();
		}
	public:
		virtual void destroy(id_t const id) = 0;
		void free_all()
		{
			std::map<id_t, optr<T>> sorted;
			for (auto& pair : this->m_objects)
				sorted.insert({ pair.first, std::move(pair.second) });
			this->m_objects.clear();
			for (auto it = sorted.rbegin(); it != sorted.rend(); it++)
				it->second.free();
		}
		template<typename U = T>
		sptr<U, T> get(id_t const id)
		{
			ASSERT(m_objects.contains(id))
			return sptr<U, T>(m_objects.at(id));
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
		std::unordered_map<id_t, optr<T>> m_objects; // map of unique id to actual object (this class owns all the objects)
	protected:
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



	class file_manager_base
	{
	public:
		file_manager_base(std::string const& base_dir) :
			m_base_dir(base_dir)
		{}
		DCM(file_manager_base);
	public:
		virtual id_t load(std::string const& fp) = 0;
		virtual void reload(std::string const& fp) = 0;
		virtual void rename(std::string const& old_fp, std::string const& new_fp) = 0;
	protected:
		std::string m_base_dir;
	protected:
		std::string to_absolute(std::string const& fp)
		{
			return u::to_absolute(m_base_dir, fp);
		}
	};



	template<typename T>
	class file_manager : public manager<T>, public file_manager_base
	{
	public:
		file_manager(std::string const& base_dir) :
			file_manager_base(base_dir)
		{}
		DCM(file_manager);
	};



	template<typename T>
	class ref_counted_manager : public manager<T>
	{
	public:
		ref_counted_manager() {}
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



	template<typename T>
	class ref_counted_file_manager : public ref_counted_manager<T>, public file_manager_base
	{
	public:
		ref_counted_file_manager(std::string const& base_dir) :
			file_manager_base(base_dir)
		{}
		DCM(ref_counted_file_manager);
	};
} // namespace djinn

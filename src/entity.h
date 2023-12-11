#pragma once
#include "component.h"
#include <map>

class entity
{
public:
	template<typename T, typename ... ARGS>
	T* add_component(ARGS&& ... args)
	{
		const component_key& k = T::get_key();
		ASSERT(!m_components.contains(k));
		T* const c = new T(std::move(args)...);
		m_components.insert({ k, c });
		return c;
	}
	template<typename T>
	T* get_component()
	{
		const component_key& k = T::get_key();
		ASSERT(m_components.contains(k));
		component* const c = m_components.at(k);
		return static_cast<T*>(c);
	}
	template<typename T>
	T const* get_component() const
	{
		const component_key& k = T::get_key();
		ASSERT(m_components.contains(k));
		component const* const c = m_components.at(k);
		return static_cast<T const*>(c);
	}
	template<typename T>
	T* remove_component()
	{
		const component_key& k = T::get_key();
		ASSERT(m_components.contains(k));
		T* const c = static_cast<T*>(m_components.at(k));
		m_components.erase(k);
		return c;
	}
	void run()
	{
		for (const auto& pair : m_components)
		{
			pair.second->run();
		}
	}
private:
	std::map<component_key, component*, component_comparator> m_components;
};
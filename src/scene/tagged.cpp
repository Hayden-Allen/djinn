#include "pch.h"
#include "tagged.h"

namespace djinn
{
	tagged::~tagged()
	{}



	bool tagged::is_entity() const
	{
		return false;
	}
	void tagged::add_tag(std::string const& tag)
	{
		m_tags.insert(tag);
	}
	bool tagged::has_tag(std::string const& tag) const
	{
		return m_tags.contains(tag);
	}
	std::unordered_set<std::string> const& tagged::get_tags() const
	{
		return m_tags;
	}
	void tagged::remove_tag(std::string const& tag)
	{
		m_tags.erase(tag);
	}



	tagged::tagged()
	{}
} // namespace djinn

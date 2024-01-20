#pragma once
#include "pch.h"

namespace djinn
{
	class tagged
	{
	public:
		DCM(tagged);
		virtual ~tagged();
	public:
		void add_tag(std::string const& tag);
		bool has_tag(std::string const& tag) const;
		std::unordered_set<std::string> const& get_tags() const;
		void remove_tag(std::string const& tag);
	protected:
		std::unordered_set<std::string> m_tags;
	protected:
		tagged();
	};
} // namespace djinn

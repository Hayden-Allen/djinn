#pragma once
#include "pch.h"

namespace djinn
{
	class xport
	{
	public:
		DCM(xport);
	public:
		std::string const& get_name() const;
		bool has_tag(std::string const& tag) const;
	protected:
		std::string m_name;
		std::unordered_set<std::string> m_tags;
	protected:
		xport();
		xport(mgl::input_file* const in);
	};
} // namespace djinn

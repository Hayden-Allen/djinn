#pragma once
#include "pch.h"
#include "scene/tagged.h"

namespace djinn
{
	class xport : public tagged
	{
	public:
		DCM(xport);
	public:
		std::string const& get_name() const;
	protected:
		std::string m_name;
	protected:
		xport();
		xport(haul::input_file* const in);
	};
} // namespace djinn

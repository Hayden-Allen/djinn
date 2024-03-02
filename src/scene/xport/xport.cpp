#include "pch.h"
#include "xport.h"

namespace djinn
{
	std::string const& xport::get_name() const
	{
		return m_name;
	}



	xport::xport()
	{}
	xport::xport(haul::input_file* const in)
	{
		m_name = in->get_string();
		u64 const tag_count = in->get64();
		m_tags.reserve(tag_count);
		for (u64 i = 0; i < tag_count; i++)
			m_tags.emplace(in->get_string());
	}
} // namespace djinn

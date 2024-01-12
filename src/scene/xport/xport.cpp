#include "pch.h"
#include "xport.h"

namespace djinn
{
	std::string const& xport::get_name() const
	{
		return m_name;
	}
	bool xport::has_tag(std::string const& tag) const
	{
		return m_tags.contains(tag);
	}



	xport::xport()
	{}
	xport::xport(mgl::input_file* const in)
	{
		m_name = in->string();
		u64 const tag_count = in->ulong();
		m_tags.reserve(tag_count);
		for (u64 i = 0; i < tag_count; i++)
			m_tags.emplace(in->string());
	}
} // namespace djinn

#include "pch.h"
#include "shaders.h"
#include "core/constants.h"

namespace djinn
{
	shaders::shaders(std::string const& vert_fp, std::string const& frag_fp, bool const fp_is_src) :
		mgl::shaders(vert_fp, frag_fp, fp_is_src)
	{
		std::unordered_set<std::string> seen;
		for_each_uniform([&](shader_uniform const& res)
			{
				if (res.name.starts_with(c::uniform::instanced_transforms_block))
				{
					std::string const& field_name = res.name.substr(res.name.find_last_of('.') + 1);
					if (!seen.contains(field_name))
					{
						m_instance_fields.emplace_back(
							field_name,
							res.type,
							res.count);
						seen.insert(field_name);
					}
				}
			});
	}
} // namespace djinn

#pragma once
#include "pch.h"

namespace djinn
{
	class shaders final : public mgl::shaders
	{
	public:
		struct instance_field
		{
			std::string name;
			GLenum type;
			s32 arr_count;
		};
	public:
		shaders(std::string const& vert_fp, std::string const& frag_fp, bool const fp_is_src = false);
		DCM(shaders);
	public:
		std::vector<instance_field> const& get_instance_fields() const
		{
			return m_instance_fields;
		}
	private:
		std::vector<instance_field> m_instance_fields;
	};
} // namespace djinn
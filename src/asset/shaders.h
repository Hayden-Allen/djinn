#pragma once
#include "core/constants.h"
#include "core/util.h"
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
			u32 offset_bytes;
		};
		enum class shader_type
		{
			NONE = 0,
			BASIC,
			ANIMATED,
			CUSTOM,
			COUNT
		};
	public:
		shaders(std::string const& vert_fp, std::string const& frag_fp);
		DCM(shaders);
	public:
		std::vector<instance_field> const& get_instance_fields() const;
		u32 get_base_offset_bytes() const;
	private:
		static inline constexpr u32 s_base_offset_bytes[(u64)shader_type::COUNT] = { 0, 64, 64 * 16, 64 };
	private:
		std::vector<instance_field> m_instance_fields;
		shader_type m_type;
	private:
		static std::vector<std::string> extract_args(std::string const& line);
		static shader_type string_to_shader_type(std::string const& type);
	private:
		std::string preprocess(std::string const& fp);
	};
} // namespace djinn
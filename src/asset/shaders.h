#pragma once
#include "core/constants.h"
#include "core/util.h"
#include "pch.h"

namespace djinn
{
	class camera_entity;

	class shaders final : public haul::parent<mgl::shaders>
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
			STATIC,
			ANIMATED,
			CUSTOM,
			PHORM,
			COUNT
		};
	public:
		shaders(std::string const& vert_afp, std::string const& frag_afp, camera_entity* const cam);
		DCM(shaders);
	public:
		std::vector<instance_field> const& get_instance_fields() const;
		u32 get_base_offset_bytes() const;
		void init(std::string const& vert_afp, std::string const& frag_afp);
		bool uniform_block_binding(std::string const& name, s32 const slot) const override;
		void bind_camera(camera_entity* const cam);
		void update_camera_uniforms();
	private:
		// sizeof(mat4) == 64
		// sizeof(mat3) == sizeof(mat3x4) == 48 (alignment)
		static inline constexpr u32 s_base_offset_bytes[(u64)shader_type::COUNT] = {
			0,
			64 + 48,						// mat4 model + mat3 normal
			64 + 64 * c::shader::num_bones, // mat4 model + mat4[] bones
			64 + 48							// mat4 model + mat3 normal
		};
	private:
		std::string m_vert_afp, m_frag_afp;
		std::vector<instance_field> m_instance_fields;
		camera_entity* m_camera;
		shader_type m_type;
	private:
		static std::vector<std::string> extract_args(std::string const& line);
		static shader_type string_to_shader_type(std::string const& type);
	private:
		std::string preprocess_vert(std::string const& fp);
		std::string preprocess_frag(std::string const& fp);
		void pushf(std::vector<std::string>* const lines, char const* const fmt, ...)
		{
			va_list args;
			va_start(args, fmt);
			char buf[512] = { 0 };
			vsprintf_s(buf, 512, fmt, args);
			lines->push_back(buf);
			va_end(args);
		}
	};
} // namespace djinn

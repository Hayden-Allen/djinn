#include "pch.h"
#include "shaders.h"
#include "core/constants.h"

namespace djinn
{
	shaders::shaders(std::string const& vert_fp, std::string const& frag_fp, bool const fp_is_src) :
		mgl::shaders(vert_fp, frag_fp, fp_is_src)
	{
		std::unordered_set<std::string> seen;
		u32 constexpr UNIFORM_TMP_SIZE = 512;
		char uniform_tmp[UNIFORM_TMP_SIZE] = { 0 };
		s32 num_uniforms;
		glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &num_uniforms);
		for (u32 i = 0; i < (u32)num_uniforms; i++)
		{
			GLenum type;
			s32 name_len, count;
			glGetActiveUniform(m_id, i, UNIFORM_TMP_SIZE - 1, &name_len, &count, &type, uniform_tmp);

			s32 block_index;
			glGetActiveUniformsiv(m_id, 1, &i, GL_UNIFORM_BLOCK_INDEX, &block_index);
			if (block_index == -1)
				continue;

			std::string name = uniform_tmp;
			ASSERT(name.contains('.'));
			std::string const& field_name = name.substr(name.find_last_of('.') + 1);
			if (seen.contains(field_name) || field_name == c::uniform::instance_transform)
				continue;
			seen.insert(field_name);

			s32 offset_bytes;
			glGetActiveUniformsiv(m_id, 1, &i, GL_UNIFORM_OFFSET, &offset_bytes);

			m_instance_fields.emplace_back(field_name, type, count, offset_bytes);
		}
	}
} // namespace djinn

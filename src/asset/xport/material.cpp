#include "pch.h"
#include "material.h"
#include "core/constants.h"

namespace djinn
{
	material::material(haul::input_file* const in, std::vector<sptr<retained_texture2d_rgba_u8_array>> const& tex)
	{
		m_use_alpha = in->get8();
		m_use_lighting = in->get8();
		m_should_cull = in->get8();

		for (u32 i = 0; i < c::shader::num_phorm_textures; i++)
		{
			u64 const idx = in->get64();
			ASSERT(idx < tex.size())
			m_textures.push_back(tex[idx]);
		}
	}



	void material::bind() const
	{
		for (u64 i = 0; i < m_textures.size(); i++)
			if (m_textures[i])
				m_textures[i]->bind((u32)i);
		if (!m_should_cull)
			glDisable(GL_CULL_FACE);
	}
	void material::unbind() const
	{
		for (u64 i = 0; i < m_textures.size(); i++)
			if (m_textures[i])
				m_textures[i]->unbind();
		if (!m_should_cull)
			glEnable(GL_CULL_FACE);
	}
	bool material::use_alpha() const
	{
		return m_use_alpha;
	}
	bool material::use_lighting() const
	{
		return m_use_lighting;
	}
} // namespace djinn

#include "pch.h"
#include "material.h"

namespace djinn
{
	material::material(mgl::input_file* const in, std::vector<sptr<texture>> const& tex)
	{
		m_use_alpha = in->ubyte();
		m_use_lighting = in->ubyte();
		m_should_cull = in->ubyte();

		for (u32 i = 0; i < 4; i++)
		{
			u64 const idx = in->ulong();
			ASSERT(idx < tex.size())
			m_textures.push_back(tex[idx]);
		}
	}
	void material::bind() const
	{
		for (u64 i = 0; i < m_textures.size(); i++)
			if (m_textures[i])
				m_textures[i]->bind((u32)i);
	}
} // namespace djinn

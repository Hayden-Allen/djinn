#include "pch.h"
#include "material.h"

namespace djinn
{
	material::material(mgl::input_file* const in, std::vector<id_t> const& tex_ids)
	{
		m_use_alpha = in->ubyte();
		m_use_lighting = in->ubyte();
		m_should_cull = in->ubyte();
		for (u32 i = 0; i < 4; i++)
		{
			u64 const idx = in->ulong();
			if (idx == 0)
			{
				m_textures[i] = 0;
			}
			else
			{
				ASSERT(idx <= tex_ids.size())
				m_textures[i] = tex_ids[idx - 1];
			}
		}
	}
} // namespace djinn

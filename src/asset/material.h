#pragma once
#include "pch.h"

namespace djinn
{
	class material
	{
	public:
		material(mgl::input_file* const in, std::vector<id_t> const& tex_ids);
		DCM(material);
	public:
		void bind() const;
	private:
		bool m_use_alpha, m_use_lighting, m_should_cull;
		std::array<texture*, 4> m_textures = { nullptr };
	};
} // namespace djinn

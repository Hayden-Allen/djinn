#pragma once
#include "pch.h"

namespace djinn
{
	class material
	{
	public:
		material(mgl::input_file* const in, std::vector<sptr<retained_texture2d_rgba_u8_array>> const& tex);
		DCM(material);
	public:
		void bind() const;
		void unbind() const;
		bool use_alpha() const;
		bool use_lighting() const;
	private:
		bool m_use_alpha, m_use_lighting, m_should_cull;
		std::vector<wptr<retained_texture2d_rgba_u8_array>> m_textures;
	};
} // namespace djinn

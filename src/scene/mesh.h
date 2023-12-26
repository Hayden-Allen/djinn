#pragma once
#include "pch.h"
#include "identifiable.h"

namespace djinn
{
	class mesh final
	{
	public:
		mesh(id_t const id);
		DCM(mesh);
		~mesh();
	private:
		static_render_object m_ro;
		std::vector<texture const*> m_textures;
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "scene_object.h"

namespace djinn
{
	class shaders;

	class phorm final : public scene_object
	{
	public:
		phorm(id_t const id, mgl::input_file* const in);
		DCM(phorm);
	public:
		void draw(sptr<mgl::context> const& ctx) const;
		void set_shaders(sptr<shaders> const& shaders);
	private:
		static_retained_render_object m_ro;
		wptr<shaders> m_shaders;
	};
} // namespace djinn

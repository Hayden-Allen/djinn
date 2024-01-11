#pragma once
#include "pch.h"
#include "visibility_scene_object.h"
#include "xport.h"

namespace djinn
{
	class shaders;

	class phorm final : public visibility_scene_object, public xport
	{
	public:
		phorm(id_t const id, mgl::input_file* const in);
		DCM(phorm);
	public:
		void draw(sptr<mgl::context> const& ctx);
		void set_shaders(sptr<shaders> const& shaders);
	private:
		static_retained_render_object m_ro;
		wptr<shaders> m_shaders;
	};
} // namespace djinn

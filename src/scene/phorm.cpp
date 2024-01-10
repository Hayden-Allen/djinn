#include "pch.h"
#include "phorm.h"
#include "asset/shaders.h"

namespace djinn
{
	phorm::phorm(id_t const id, mgl::input_file* const in) :
		visibility_scene_object(id),
		m_shaders(nullptr)
	{
		m_ro.load(*in);
	}



	void phorm::draw(sptr<mgl::context> const& ctx) const
	{
		ASSERT(m_shaders);
		ctx->draw(m_ro, *m_shaders.get());
	}
	void phorm::set_shaders(sptr<shaders> const& shaders)
	{
		m_shaders = shaders;
	}
} // namespace djinn

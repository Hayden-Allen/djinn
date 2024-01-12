#include "pch.h"
#include "phorm.h"
#include "asset/shaders.h"

namespace djinn
{
	phorm::phorm(id_t const id, mgl::input_file* const in) :
		visibility_scene_object(id),
		m_shaders(nullptr)
	{
		in->read(m_transform.e, 16);
		// TODO this is object2world
		extract_transform(m_transform);

		u64 const ro_count = in->ulong();
		for (u64 i = 0; i < ro_count; i++)
		{
			u32 const idx = in->uint();
			static_retained_render_object ro(*in);
			m_ros.emplace(idx, std::move(ro));
		}
	}



	void phorm::draw(sptr<mgl::context> const& ctx)
	{
		ASSERT(m_shaders);
		if (m_visible)
		{
			update_transform();
			tmat<space::OBJECT, space::WORLD> const& model = get_graphics_transform();
			if (m_shaders->has_uniform(c::uniform::model_mat))
				m_shaders->uniform_mat4(c::uniform::model_mat, model.e);
			if (m_shaders->has_uniform(c::uniform::normal_mat))
			{
				std::vector<f32> const& normal = model.invert_copy().transpose_copy().mat3();
				m_shaders->uniform_mat3(c::uniform::normal_mat, normal.data());
			}
			for (auto const& pair : m_ros)
				ctx->draw(pair.second, *m_shaders.get());
		}
	}
	void phorm::set_shaders(sptr<shaders> const& shaders)
	{
		m_shaders = shaders;
	}
} // namespace djinn

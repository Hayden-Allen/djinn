#include "pch.h"
#include "phorm.h"
#include "asset/shaders.h"
#include "asset/xport/material.h"
#include "script/service/render_service.h"

namespace djinn
{
	phorm::phorm(id_t const id, haul::input_file* const in, std::unordered_map<u32, sptr<material>> const& mats) :
		visibility_scene_object(id),
		xport(in),
		m_shaders(nullptr),
		m_alpha_shaders(nullptr)
	{
		u64 const ro_count = in->get64();
		for (u64 i = 0; i < ro_count; i++)
		{
			u32 const material_idx = in->get32();
			ASSERT(mats.contains(material_idx));
			m_ros.insert({ mats.at(material_idx).get(), std::move(static_retained_render_object(in)) });
		}
	}



	void phorm::draw(sptr<mgl::context> const& ctx)
	{
		// must have at least one type of shader bound to draw
		ASSERT(m_shaders || m_alpha_shaders);
		if (m_visible)
		{
			// update_transform();
			if (m_shaders)
				shader_setup(m_shaders);
			if (m_alpha_shaders)
				shader_setup(m_alpha_shaders);

			for (auto const& pair : m_ros)
			{
				pair.first->bind();
				// draw with appropriate shaders for each material
				if (pair.first->use_alpha())
				{
					ASSERT(m_alpha_shaders);
					m_alpha_shaders->uniform_1i(c::uniform::phorm_use_lighting, pair.first->use_lighting());
					ctx->draw(pair.second, *m_alpha_shaders.get());
				}
				else
				{
					ASSERT(m_shaders);
					m_shaders->uniform_1i(c::uniform::phorm_use_lighting, pair.first->use_lighting());
					ctx->draw(pair.second, *m_shaders.get());
				}
				pair.first->unbind();
			}
		}
	}
	void phorm::set_shaders(sptr<shaders> const& shaders)
	{
		m_shaders = shaders;
	}
	void phorm::set_alpha_shaders(sptr<shaders> const& alpha_shaders)
	{
		m_alpha_shaders = alpha_shaders;
	}
	std::unordered_map<material const*, static_retained_render_object> const& phorm::get_render_objects() const
	{
		return m_ros;
	}
	u64 const phorm::get_vertex_count() const
	{
		u64 count = 0;
		for (auto const& pair : m_ros)
		{
			count += pair.second.get_vertex_array().get_vertex_count();
		}
		return count;
	}



	void phorm::shader_setup(wptr<shaders> shaders)
	{
		tmat<space::OBJECT, space::WORLD> const& model = get_graphics_transform();
		shaders->uniform_mat4(c::uniform::model_mat, model.e);

		f32 normal[9] = { 0 };
		model.invert_copy().transpose_copy().mat3(normal);
		shaders->uniform_mat3(c::uniform::normal_mat, normal);

		shaders->uniform_1f(c::uniform::time, render_service::get_context()->time.now);
		// done here for hot reloading
		for (u32 i = 0; i < c::shader::num_phorm_textures; i++)
			shaders->uniform_1i(c::uniform::phorm_textures[i], i);
	}
} // namespace djinn

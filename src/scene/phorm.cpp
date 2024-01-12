#include "pch.h"
#include "phorm.h"
#include "asset/shaders.h"
#include "asset/material.h"
#include "script/service/render_service.h"

namespace djinn
{
	phorm::phorm(id_t const id, mgl::input_file* const in, std::unordered_map<u32, sptr<material>> const& mats) :
		visibility_scene_object(id),
		xport(in),
		m_shaders(nullptr)
	{
		// TODO matrix stored in the file is object2world
		in->read(m_transform.e, 16);
		extract_transform(m_transform);

		u64 const ro_count = in->ulong();
		for (u64 i = 0; i < ro_count; i++)
		{
			u32 const material_idx = in->uint();
			printf("\t%zu\n", in->get_pos());
			printf("\t%u | %u(%zu)\n", id, material_idx, i);
			ASSERT(mats.contains(material_idx));
			m_ros.emplace(mats.at(material_idx), *in);
		}
	}



	void phorm::draw(sptr<mgl::context> const& ctx)
	{
		ASSERT(m_shaders);
		if (m_visible)
		{
			update_transform();
			tmat<space::OBJECT, space::WORLD> const& model = get_graphics_transform();
			m_shaders->uniform_mat4(c::uniform::model_mat, model.e);
			std::vector<f32> const& normal = model.invert_copy().transpose_copy().mat3();
			m_shaders->uniform_mat3(c::uniform::normal_mat, normal.data());

			// done here for hot reloading
			for (u32 i = 0; i < 4; i++)
				m_shaders->uniform_1i(c::uniform::phorm_textures[i], i);
			m_shaders->uniform_1f(c::uniform::time, render_service::get_context()->time.now);
			for (auto const& pair : m_ros)
			{
				pair.first->bind();
				ctx->draw(pair.second, *m_shaders.get());
			}
		}
	}
	void phorm::set_shaders(sptr<shaders> const& shaders)
	{
		m_shaders = shaders;
	}
} // namespace djinn

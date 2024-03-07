#pragma once
#include "pch.h"
#include "mesh_instance.h"
#include "asset/mesh/mesh.h"
#include "asset/shaders.h"
#include "script/service/render_service.h"

namespace djinn
{
	class mesh_instance_batch final
	{
	public:
		mesh_instance_batch(wptr<mesh> const& mesh, wptr<shaders> const& shaders);
		mesh_instance_batch(mesh_instance_batch const&) = delete;
		mesh_instance_batch(mesh_instance_batch&& other) noexcept;
	public:
		bool is_full() const;
		bool empty() const;
		u64 insert(sptr<mesh_instance> instance);
		void remove(u64 const index);
		void update(u64 const index, mesh_instance_field const& field);
		template<typename RO>
		void draw(sptr<mgl::context> const& ctx, RO const& ro)
		{
			bool changed[c::shader::num_batch_ubos] = { false };
			for (u64 i = 0; i < m_instances.size(); i++)
			{
				// m_instances[i]->update_transform();
				if (m_mesh.get()->is_animated() || m_instances[i]->is_transform_dirty())
				{
					update_transform(i, m_instances[i]->get_graphics_transform());
					m_instances[i]->set_transform_clean();
					u64 const ubo_index = i / m_instances_per_ubo;
					changed[ubo_index] = true;
				}
			}
			for (u64 i = 0; i < c::shader::num_batch_ubos; i++)
			{
				if (changed[i])
				{
					u32 const floats_per_ubo = m_floats_per_instance * m_instances_per_ubo;
					m_ubos[i].update(m_buffer.data() + i * floats_per_ubo, floats_per_ubo, 0);
				}
			}

			for (u32 i = 0; i < (u32)m_ubos.size(); i++)
			{
				m_ubos[i].bind(i);
			}
			m_shaders->uniform_1f(c::uniform::time, render_service::get_context()->time.now);
			ctx->draw_instanced(ro, *m_shaders.get(), m_valid);
		}
	private:
		static inline constexpr u32 s_floats_per_mat4 = 16, s_floats_per_mat3 = 12;
	private:
		wptr<mesh> m_mesh;
		wptr<shaders> m_shaders;
		std::vector<sptr<mesh_instance>> m_instances; // max size = num_batch_ubos * s_transforms_per_ubo
		std::vector<dynamic_uniform_buffer> m_ubos;	  // max size = num_batch_ubos
		std::vector<u64> m_openings;				  // empty slots in m_instances
		std::vector<u64> m_instance_indices;		  // same size as m_instances. element at i is where m_instances[i] writes its transform in m_ubos
		std::vector<u64> m_max_instance_index;		  // size = m_valid, stores the index of the max value in m_transform_indices
		std::vector<f32> m_buffer;					  // size = m_floats_per_instance * m_instances_per_ubo * m_ubos.size()
		s32 m_next = 0, m_valid = 0;				  // end of m_instances, number of valid ids in m_instances
		u32 m_instances_per_ubo = 0, m_floats_per_instance = 0;
	private:
		void add_block();
		void set_transform_index(u64 const index);
		void update_transform(u64 const index, tmat<space::OBJECT, space::WORLD> const& transform);
		void write_to_buffer(u64 const block_index, u64 const offset_floats, f32 const* const data, u64 const float_count);
	};
} // namespace djinn

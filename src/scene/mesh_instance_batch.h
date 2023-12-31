#pragma once
#include "pch.h"

namespace djinn
{
	class mesh;
	class mesh_instance;
	struct mesh_instance_field;
	class shaders;

	class mesh_instance_batch final
	{
	public:
		mesh_instance_batch(wptr<mesh> const& mesh, wptr<shaders> const& shaders);
		mesh_instance_batch(mesh_instance_batch const&) = delete;
		mesh_instance_batch(mesh_instance_batch&& other) noexcept;
	public:
		bool empty() const;
		u64 insert(sptr<mesh_instance> instance);
		void remove(u64 const index);
		void update(u64 const index, mesh_instance_field const& field);
		void draw(sptr<mgl::context> const& ctx, static_render_object const& ro);
	private:
		static inline constexpr u32 s_num_ubos = 12;
	private:
		wptr<mesh> m_mesh;
		wptr<shaders> m_shaders;
		std::vector<sptr<mesh_instance>> m_instances; // max size = s_num_ubos * s_transforms_per_ubo
		std::vector<dynamic_uniform_buffer> m_ubos;	  // max size = s_num_ubos
		std::vector<u64> m_openings;				  // empty slots in m_instances
		std::vector<u64> m_instance_indices;		  // same size as m_instances. element at i is where m_instances[i] writes its transform in m_ubos
		std::vector<u64> m_max_instance_index;		  // size = m_valid, stores the index of the max value in m_transform_indices
		s32 m_next = 0, m_valid = 0;				  // end of m_instances, number of valid ids in m_instances
		u32 m_instances_per_ubo = 0, m_floats_per_instance = 0;
	private:
		void add_block();
		void set_transform_index(u64 const index);
		void update_transform(u64 const index, tmat<space::OBJECT, space::WORLD> const& transform);
	};
} // namespace djinn

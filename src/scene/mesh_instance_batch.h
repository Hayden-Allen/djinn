#pragma once
#include "pch.h"

namespace djinn
{
	class mesh;
	class mesh_instance;

	class mesh_instance_batch final
	{
	public:
		mesh_instance_batch(wptr<mesh> const& mesh);
		DCM(mesh_instance_batch);
	public:
		u64 get_count() const;
		u64 insert(sptr<mesh_instance> instance);
		void remove(u64 const index);
		void update(u64 const index, tmat<space::OBJECT, space::WORLD> const& transform);
		void draw(sptr<mgl::context> const& ctx, static_render_object const& ro, sptr<shaders> const& shaders);
	private:
		static inline u32 const s_floats_per_mat = 16;
		static inline u32 const s_mat_size = s_floats_per_mat * sizeof(float);
		static inline u32 s_num_ubos;
		static inline u32 s_transforms_per_ubo;
	private:
		wptr<mesh> m_mesh;
		std::vector<sptr<mesh_instance>> m_instances;	  // max size = s_num_ubos * s_transforms_per_ubo
		std::vector<dynamic_uniform_buffer> m_transforms; // max size = s_num_ubos, each size = s_floats_per_mat * s_transforms_per_ubo
		std::vector<u64> m_openings;					  // empty slots in m_instances
		std::vector<u64> m_transform_indices;			  // same size as m_instances. element at i is where m_instances[i] writes its transform in m_transforms
		std::vector<u64> m_max_transform_index;			  // size = m_valid, stores the index of the max value in m_transform_indices
		s32 m_next = 0, m_valid = 0;					  // end of m_instances, number of valid ids in m_instances
	private:
		void add_block();
		void set_transform_index(u64 const index);
	};
} // namespace djinn

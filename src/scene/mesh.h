#pragma once
#include "pch.h"
#include "identifiable.h"
#include "mesh_instance_batch.h"

namespace djinn
{
	class mesh_instance;
	class shaders;

	class mesh final
	{
		friend class mesh_instance_manager;
		friend class mesh_instance;
	public:
		mesh(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<sptr<texture>> const& textures);
		DCM(mesh);
	public:
		void update(std::vector<f32> const& vertices, std::vector<u32> const& indices);
		void draw(sptr<mgl::context> const& ctx);
	private:
		static_render_object m_ro;
		std::vector<sptr<texture>> m_textures;
		std::unordered_map<sptr<shaders>, mesh_instance_batch> m_batches;
	private:
		void insert_instance(sptr<shaders> const& shaders, sptr<mesh_instance> instance);
		void remove_instance(sptr<shaders> const& shaders, u64 const index);
	};
} // namespace djinn

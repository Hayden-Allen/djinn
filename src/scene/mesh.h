#pragma once
#include "pch.h"
#include "identifiable.h"
#include "mesh_instance_batch.h"

namespace djinn
{
	class mesh_instance;

	class mesh final
	{
		friend class mesh_instance_manager;
	public:
		mesh(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<sptr<texture>> const& textures, sptr<shaders> const& shaders);
		DCM(mesh);
	public:
		void update(std::vector<f32> const& vertices, std::vector<u32> const& indices);
		void draw(sptr<mgl::context> const& ctx);
		void set_shaders(sptr<shaders> const& shaders);
	private:
		static_render_object m_ro;
		std::vector<sptr<texture>> m_textures;
		mesh_instance_batch m_batch;
		sptr<shaders> m_shaders;
	private:
		void insert_instance(sptr<mesh_instance> instance);
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "mesh.h"

namespace djinn
{
	struct custom_mesh_texture
	{
		union
		{
			retained_texture2d_rgba_u8_array* arr;
			texture* raw;
		};
		bool is_raw;
	};

	class custom_mesh final : public mesh
	{
	public:
		custom_mesh(u32 const vertex_count, std::vector<u32> const& layout, u32 const index_count, std::vector<custom_mesh_texture> const& textures);
		DCM(custom_mesh);
	public:
		void update(std::vector<f32> const& vertices);
		void update(std::vector<u32> const& indices);
		void update(std::vector<f32> const& vertices, std::vector<u32> const& indices);
		void draw(sptr<mgl::context> const& ctx) override final;
	private:
		static_render_object m_ro;
		std::vector<custom_mesh_texture> m_textures;
	};
} // namespace djinn

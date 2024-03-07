#pragma once
#include "pch.h"

namespace djinn
{
	class mesh_instance;
	class mesh_instance_batch;
	class shaders;

	struct mesh_texture
	{
		union
		{
			retained_texture2d_rgba_u8_array* arr;
			texture* raw;
		};
		bool is_raw;
	};

	class mesh
	{
		friend class mesh_instance_manager;
		friend class mesh_instance;
	public:
		mesh(std::vector<mesh_texture> const& textures);
		DCM(mesh);
		virtual ~mesh();
	public:
		virtual void draw(sptr<mgl::context> const& ctx);
		virtual bool is_animated() const;
	protected:
		std::unordered_map<shaders const*, std::vector<mesh_instance_batch*>> m_batches;
		std::unordered_map<shaders const*, u64> m_num_empty_batches;
		std::vector<mesh_texture> m_textures;
	protected:
		mesh();
	protected:
		void insert_instance(wptr<shaders> const& shaders, sptr<mesh_instance> instance);
		void remove_instance(wptr<shaders> const& shaders, u64 const batch_number, u64 const index);
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "scene/mesh_instance_batch.h"

namespace djinn
{
	class mesh_instance;
	class shaders;

	class mesh
	{
		friend class mesh_instance_manager;
		friend class mesh_instance;
	public:
		DCM(mesh);
		virtual ~mesh();
	public:
		virtual void draw(sptr<mgl::context> const& ctx) = 0;
		virtual bool is_animated() const;
	protected:
		std::unordered_map<wptr<shaders>, mesh_instance_batch> m_batches;
	protected:
		mesh();
	protected:
		void insert_instance(wptr<shaders> const& shaders, sptr<mesh_instance> instance);
		void remove_instance(wptr<shaders> const& shaders, u64 const index);
	};
} // namespace djinn

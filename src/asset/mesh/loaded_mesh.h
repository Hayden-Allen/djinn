#pragma once
#include "pch.h"
#include "mesh.h"
#include "scene/mesh/mesh_instance_batch.h"

namespace djinn
{
	template<typename VERT>
	class loaded_mesh : public mesh
	{
	public:
		loaded_mesh() {}
		DCM(loaded_mesh);
		virtual ~loaded_mesh() {}
	public:
		virtual void init(m3d_t* const raw) = 0;
		void draw(sptr<mgl::context> const& ctx)
		{
			for (auto& pair : m_batches)
				for (auto& batch : pair.second)
					batch->draw(ctx, m_ro);
		}
	protected:
		static_structured_render_object<VERT> m_ro;
	protected:
		virtual std::vector<std::pair<GLenum, std::vector<u32>>> get_layout() const = 0;
	};
} // namespace djinn

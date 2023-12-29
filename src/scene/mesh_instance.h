#pragma once
#include "pch.h"
#include "scene_object.h"

namespace djinn
{
	class mesh;
	class mesh_instance_batch;
	class shaders;

	struct mesh_instance_field
	{
		std::vector<f32> data;
		GLenum type;
		s32 arr_count;
	};

	class mesh_instance final : public scene_object
	{
		friend class mesh_instance_batch;
	public:
		mesh_instance(id_t const id, wptr<mesh> const& mesh, sptr<shaders> const& batch_shaders);
		DCM(mesh_instance);
	public:
		void remove_from_batch();
	private:
		wptr<mesh> m_mesh;
		u64 m_batch_index;
		sptr<shaders> m_batch_shaders;
		std::unordered_map<std::string, mesh_instance_field> m_fields;
	private:
		void bind(u64 const index);
	};
} // namespace djinn

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
		u32 offset_bytes;
	};

	class mesh_instance final : public scene_object
	{
		friend class mesh_instance_batch;
		friend class mesh_instance_manager;
	public:
		mesh_instance(id_t const id, wptr<mesh> const& mesh, sptr<shaders> const& batch_shaders);
		DCM(mesh_instance);
	public:
		void set_uniform(std::string const& name, std::vector<f32> const& data, u32 const index);
	private:
		wptr<mesh> m_mesh;
		u64 m_batch_index;
		sptr<shaders> m_batch_shaders;
		wptr<mesh_instance_batch> m_batch;
		std::vector<mesh_instance_field> m_fields;
		std::unordered_map<std::string, u64> m_field_index;
	private:
		void bind(wptr<mesh_instance_batch> batch, u64 const index);
		void remove_from_batch();
	};
} // namespace djinn

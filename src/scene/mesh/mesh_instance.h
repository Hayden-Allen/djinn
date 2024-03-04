#pragma once
#include "pch.h"
#include "scene/visibility_scene_object.h"

namespace djinn
{
	class mesh;
	class mesh_instance_batch;
	class shaders;

	// represents a field in the per-instance uniform block
	struct mesh_instance_field
	{
		std::vector<f32> data; // raw float data
		GLenum type;		   // GLSL type
		s32 arr_count;		   // each field can be an array, this is the length
		u32 offset_bytes;	   // offset within the block
	};



	class mesh_instance : public visibility_scene_object
	{
		friend class mesh_instance_batch;
		friend class mesh_instance_manager;
		friend class mesh;
	public:
		mesh_instance(id_t const id, sptr<mesh> const& mesh, wptr<shaders> const& batch_shaders);
		DCM(mesh_instance);
	public:
		void set_uniform(std::string const& name, std::vector<f32> const& data, u32 const index);
		virtual bool is_animated() const;
	protected:
		sptr<mesh> m_mesh;
		// index of mesh_instance_batch object in mesh::m_batches, index within that batch
		u64 m_batch_number, m_batch_index;
		wptr<shaders> m_batch_shaders;
		wptr<mesh_instance_batch> m_batch;
		std::vector<mesh_instance_field> m_fields;
		std::unordered_map<std::string, u64> m_field_index;
	protected:
		void bind_mesh(u64 const batch_number);
		void bind_batch(wptr<mesh_instance_batch> batch, u64 const batch_index);
		void remove_from_batch();
	};
} // namespace djinn

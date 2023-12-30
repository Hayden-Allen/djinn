#include "pch.h"
#include "mesh_instance_manager.h"
#include "scene/mesh.h"

namespace djinn
{
	mesh_instance_manager::mesh_instance_manager() :
		haul::parent<ref_counted_manager<mesh_instance>>("")
	{}



	id_t mesh_instance_manager::create(sptr<mesh> mesh, sptr<shaders> shaders)
	{
		id_t const id = insert(new mesh_instance(s_next_id, mesh, wptr(shaders)));
		mesh->insert_instance(wptr(shaders), get(id));
		return id;
	}
	id_t mesh_instance_manager::load(std::string const& fp)
	{
		ASSERT(false);
		return 0;
	}
	void mesh_instance_manager::destroy(id_t const id)
	{
		get(id)->remove_from_batch();
		try_erase(id);
	}
	void mesh_instance_manager::reload(std::string const& fp)
	{
		ASSERT(false);
	}
	void mesh_instance_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		ASSERT(false);
	}
	void mesh_instance_manager::free_all()
	{
		for (auto& pair : m_objects)
			pair.second->remove_from_batch();
		super::free_all();
	}
} // namespace djinn
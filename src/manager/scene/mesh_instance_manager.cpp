#include "pch.h"
#include "mesh_instance_manager.h"
#include "asset/mesh/mesh.h"
#include "scene/mesh/animated_mesh_instance.h"

namespace djinn
{
	mesh_instance_manager::mesh_instance_manager()
	{}



	id_t mesh_instance_manager::create(sptr<mesh> mesh, sptr<shaders> shaders)
	{
		id_t id = 0;
		if (mesh->is_animated())
			id = insert(new animated_mesh_instance(s_next_id, mesh, wptr(shaders)));
		else
			id = insert(new mesh_instance(s_next_id, mesh, wptr(shaders)));
		mesh->insert_instance(wptr(shaders), get(id));
		return id;
	}
	void mesh_instance_manager::destroy(id_t const id)
	{
		get(id)->remove_from_batch();
		try_erase(id);
	}
} // namespace djinn

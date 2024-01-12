#include "pch.h"
#include "camera_entity_manager.h"
#include "scene/entity/camera_entity.h"
#include "core/constants.h"

namespace djinn
{
	camera_entity_manager::camera_entity_manager(JSRuntime* const runtime) :
		entity_manager_base(runtime)
	{}



	id_t camera_entity_manager::load(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		return load_base(new camera_entity(s_next_id, afp, m_runtime), afp);
	}
} // namespace djinn

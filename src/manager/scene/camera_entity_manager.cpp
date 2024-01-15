#include "pch.h"
#include "camera_entity_manager.h"
#include "scene/entity/camera_entity.h"
#include "core/constants.h"

namespace djinn
{
	camera_entity_manager::camera_entity_manager(JSRuntime* const runtime) :
		entity_manager_base(runtime),
		m_primary(0)
	{}



	id_t camera_entity_manager::load(std::string const& fp, s32 const argc, JSValueConst* const argv)
	{
		std::string const& afp = to_absolute(fp);
		id_t const id = load_base(new camera_entity(s_next_id, afp, m_runtime), afp, argc, argv);
		if (m_primary == 0)
			m_primary = id;
		return id;
	}
	sptr<camera_entity> camera_entity_manager::get_primary()
	{
		ASSERT(m_primary);
		return get(m_primary);
	}
} // namespace djinn

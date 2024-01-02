#include "pch.h"
#include "entity_manager.h"
#include "scene/entity.h"
#include "core/constants.h"

namespace djinn
{
	entity_manager::entity_manager(JSRuntime* const runtime) :
		entity_manager_base(runtime)
	{}



	id_t entity_manager::load(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		return load_base(new entity(s_next_id, afp, m_runtime), afp);
	}
} // namespace djinn

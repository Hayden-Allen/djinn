#pragma once
#include "pch.h"
#include "entity_manager_base.h"
#include "scene/entity/entity.h"

namespace djinn
{
	class entity_manager final : public entity_manager_base<entity>
	{
	public:
		entity_manager(JSRuntime* const runtime);
		DCM(entity_manager);
	public:
		id_t load(std::string const& fp) override;
	};
} // namespace djinn

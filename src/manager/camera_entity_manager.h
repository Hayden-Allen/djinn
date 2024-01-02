#pragma once
#include "pch.h"
#include "entity_manager_base.h"
#include "scene/camera_entity.h"

namespace djinn
{
	class camera_entity_manager final : public entity_manager_base<camera_entity>
	{
	public:
		camera_entity_manager(JSRuntime* const runtime);
		DCM(camera_entity_manager);
	public:
		id_t load(std::string const& fp) override;
	};
} // namespace djinn

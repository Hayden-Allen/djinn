#pragma once
#include "pch.h"
#include "directory_watcher.h"
#include "core/constants.h"

namespace djinn
{
	class entity_manager;
	class camera_entity_manager;

	class script_watcher_entity : public directory_watcher<entity_manager>
	{
	public:
		script_watcher_entity(entity_manager* const manager) :
			directory_watcher(c::base_dir::script, manager, 1000)
		{}
		DCM(script_watcher_entity);
	};
	class script_watcher_camera : public directory_watcher<camera_entity_manager>
	{
	public:
		script_watcher_camera(camera_entity_manager* const manager) :
			directory_watcher(c::base_dir::script, manager, 1000)
		{}
		DCM(script_watcher_camera);
	};
} // namespace djinn

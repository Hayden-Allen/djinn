#pragma once
#include "pch.h"
#include "directory_watcher.h"
#include "core/constants.h"

namespace djinn
{
	class entity_manager;

	class script_watcher : public directory_watcher<entity_manager>
	{
	public:
		script_watcher(entity_manager* const manager) :
			directory_watcher(c::base_dir::script, manager)
		{}
		DCM(script_watcher);
	};
} // namespace djinn

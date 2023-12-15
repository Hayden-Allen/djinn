#pragma once
#include "pch.h"
#include "directory_watcher.h"
#include "core/constants.h"

namespace djinn
{
	class script_manager;

	class script_watcher : public directory_watcher<script_manager>
	{
	public:
		script_watcher(script_manager* const manager) :
			directory_watcher(c::base_dir::script, manager)
		{}
		DCM(script_watcher);
	};
} // namespace djinn

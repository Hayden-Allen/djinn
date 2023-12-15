#pragma once
#include "pch.h"
#include "directory_watcher.h"
#include "core/constants.h"

namespace djinn
{
	class texture_manager;

	class texture_watcher : public directory_watcher<texture_manager>
	{
	public:
		texture_watcher(texture_manager* const manager) :
			directory_watcher(c::base_dir::texture, manager)
		{}
		DCM(texture_watcher);
	};
} // namespace djinn

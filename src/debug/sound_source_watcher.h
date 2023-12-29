#pragma once
#include "pch.h"
#include "directory_watcher.h"
#include "core/constants.h"

namespace djinn
{
	class sound_source_manager;

	class sound_source_watcher : public directory_watcher<sound_source_manager>
	{
	public:
		sound_source_watcher(sound_source_manager* const manager) :
			directory_watcher(c::base_dir::sound, manager)
		{}
		DCM(sound_source_watcher);
	};
} // namespace djinn

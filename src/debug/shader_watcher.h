#pragma once
#include "pch.h"
#include "directory_watcher.h"
#include "core/constants.h"

namespace djinn
{
	class shader_manager;

	class shader_watcher : public directory_watcher<shader_manager>
	{
	public:
		shader_watcher(shader_manager* const manager) :
			directory_watcher(c::base_dir::shader, manager)
		{}
		DCM(shader_watcher);
	};
} // namespace djinn

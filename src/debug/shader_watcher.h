#pragma once
#include "pch.h"
#include "directory_watcher.h"
#include "core/constants.h"

namespace djinn
{
	class shader_manager;

	class shader_watcher : public haul::parent<directory_watcher<shader_manager>>
	{
	public:
		shader_watcher(shader_manager* const manager) :
			haul::parent<directory_watcher<shader_manager>>(c::base_dir::shader, manager)
		{}
		DCM(shader_watcher);
	public:
		void handle_action(DWORD const action, std::string const& fp) override;
	};
} // namespace djinn

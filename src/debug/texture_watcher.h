#pragma once
#include "pch.h"
#include "directory_watcher.h"
#include "core/constants.h"
#include "script/manager/texture_manager.h"

namespace djinn
{
	class texture_watcher : public haul::parent<directory_watcher<texture_manager>>
	{
	public:
		texture_watcher(texture_manager* const manager) :
			haul::parent<directory_watcher<texture_manager>>(c::base_dir::texture, manager)
		{}
		DCM(texture_watcher);
	public:
		// void handle_action(DWORD const action, std::string const& fp) override;
	};
} // namespace djinn

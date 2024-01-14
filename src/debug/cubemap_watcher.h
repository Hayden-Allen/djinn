#pragma once
#include "pch.h"
#include "directory_watcher.h"
#include "core/constants.h"

namespace djinn
{
	class cubemap_manager;

	class cubemap_watcher : public haul::parent<directory_watcher<cubemap_manager>>
	{
	public:
		cubemap_watcher(cubemap_manager* const manager) :
			haul::parent<directory_watcher<cubemap_manager>>(c::base_dir::texture, manager, 0)
		{}
		DCM(cubemap_watcher);
	public:
		void handle_action(DWORD const action, std::string const& fp) override;
	};
} // namespace djinn

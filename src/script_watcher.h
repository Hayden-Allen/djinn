#pragma once
#include "pch.h"
#include "directory_watcher.h"

namespace djinn
{
	class script_manager;

	class script_watcher : public directory_watcher
	{
	public:
		script_watcher(std::string const& base_dir, script_manager* const manager);
		DCM(script_watcher);
	public:
		void handle_action(DWORD const action, std::string const& fp) override;
	private:
		std::string m_old_fp;
		script_manager* m_manager;
	};
} // namespace djinn

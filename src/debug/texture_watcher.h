#pragma once
#include "pch.h"
#include "directory_watcher.h"

namespace djinn
{
	class texture_manager;

	class texture_watcher : public directory_watcher
	{
	public:
		texture_watcher(texture_manager* const manager);
		DCM(texture_watcher);
	public:
		void handle_action(DWORD const action, std::string const& fp) override;
	private:
		std::string m_old_fp;
		texture_manager* m_manager;
	};
} // namespace djinn

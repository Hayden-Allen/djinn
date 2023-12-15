#pragma once
#include "pch.h"
#include "directory_watcher.h"

namespace djinn
{
	class shader_manager;

	class shader_watcher : public directory_watcher
	{
	public:
		shader_watcher(shader_manager* const manager);
		DCM(shader_watcher);
	public:
		void handle_action(DWORD const action, std::string const& fp) override;
	private:
		std::string m_old_fp;
		shader_manager* m_manager;
	};
} // namespace djinn

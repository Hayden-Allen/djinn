#include "pch.h"
#include "script_watcher.h"
#include "script_manager.h"

script_watcher::script_watcher(std::string const& base_dir, script_manager* const manager) :
	directory_watcher(base_dir),
	m_manager(manager)
{}



void script_watcher::handle_action(DWORD const action, std::string const& fp)
{
	switch (action)
	{
	case FILE_ACTION_MODIFIED:
		m_manager->reload_file(fp);
		break;
	case FILE_ACTION_REMOVED:
	case FILE_ACTION_RENAMED_OLD_NAME:
		m_old_fp = fp;
		break;
	case FILE_ACTION_ADDED:
	case FILE_ACTION_RENAMED_NEW_NAME:
		m_manager->rename_file(m_old_fp, fp);
		break;
	}
}

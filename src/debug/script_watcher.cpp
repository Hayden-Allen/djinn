#include "pch.h"
#include "script_watcher.h"
#include "scene/script_manager.h"
#include "core/constants.h"

namespace djinn
{
	script_watcher::script_watcher(script_manager* const manager) :
		directory_watcher(c::base_dir::script),
		m_manager(manager)
	{}



	void script_watcher::handle_action(DWORD const action, std::string const& fp)
	{
		switch (action)
		{
		case FILE_ACTION_MODIFIED:
			m_manager->reload(fp);
			break;
		case FILE_ACTION_REMOVED:
		case FILE_ACTION_RENAMED_OLD_NAME:
			m_old_fp = fp;
			break;
		case FILE_ACTION_ADDED:
		case FILE_ACTION_RENAMED_NEW_NAME:
			m_manager->rename(m_old_fp, fp);
			break;
		}
	}
} // namespace djinn

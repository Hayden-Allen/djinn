#include "pch.h"
#include "shader_watcher.h"
#include "script/shader_manager.h"
#include "core/constants.h"

namespace djinn
{
	djinn::shader_watcher::shader_watcher(shader_manager* const manager) :
		directory_watcher(c::base_dir::shader),
		m_manager(manager)
	{}



	void djinn::shader_watcher::handle_action(DWORD const action, std::string const& fp)
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

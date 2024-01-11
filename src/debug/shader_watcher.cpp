#include "pch.h"
#include "shader_watcher.h"
#include "core/util.h"
#include "manager/shader_manager.h"

namespace djinn
{
	void shader_watcher::handle_action(DWORD const action, std::string const& fp)
	{
		if (action == FILE_ACTION_MODIFIED)
			u::sleep(500);
		super::handle_action(action, fp);
	}
} // namespace djinn

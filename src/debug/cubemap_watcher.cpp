#include "pch.h"
#include "cubemap_watcher.h"
#include "core/util.h"
#include "script/manager/cubemap_manager.h"

namespace djinn
{
	void cubemap_watcher::handle_action(DWORD const action, std::string const& fp)
	{
		if (action == FILE_ACTION_MODIFIED)
			u::sleep(100);
		super::handle_action(action, fp);
	}
} // namespace djinn

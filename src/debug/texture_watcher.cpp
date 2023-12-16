#include "pch.h"
#include "texture_watcher.h"
#include "core/util.h"

namespace djinn
{
	void texture_watcher::handle_action(DWORD const action, std::string const& fp)
	{
		if (action == FILE_ACTION_MODIFIED)
			u::sleep(100);
		super::handle_action(action, fp);
	}
} // namespace djinn

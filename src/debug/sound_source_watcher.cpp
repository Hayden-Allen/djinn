#include "pch.h"
#include "sound_source_watcher.h"
#include "core/util.h"
#include "script/manager/sound_source_manager.h"

namespace djinn
{
	void sound_source_watcher::handle_action(DWORD const action, std::string const& fp)
	{
		if (action == FILE_ACTION_MODIFIED)
			u::sleep(3000);
		super::handle_action(action, fp);
	}
} // namespace djinn

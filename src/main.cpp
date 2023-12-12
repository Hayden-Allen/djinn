#include "pch.h"
#include "script_watcher.h"

// #include "entity2.h"
#include "script_manager.h"
#include "entity.h"
// #include "component.h"
script_manager* g_all_scripts;
// JSRuntime* g_js_runtime;

int main(int argc, char* argv[])
{
	static char constexpr WATCH_BASE_DIR[] = "../../../../cwd/res/scripts/out";
	g_all_scripts = new script_manager(WATCH_BASE_DIR);
	script_watcher watcher(WATCH_BASE_DIR, g_all_scripts);
	entity* const e = g_all_scripts->create_entity("test.js");


	while (!(GetKeyState('Q') & 0x8000))
	{
		// non blocking wait
		watcher.poll();

		e->update();
	}

	int const x = 4;

	g_all_scripts->remove_entity(e);
	delete e;
	delete g_all_scripts;

	return 0;
}
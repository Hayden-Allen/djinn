#ifdef _DEBUG
#define ASSERT(x) if(!(x)) __debugbreak();
#else
#define ASSERT(x)
#endif
#define MAX_VALUE_T(T) std::numeric_limits<T>::max();

#define QJS_USE_CPP
#include <quickjs.h>
#include <iostream>
#include "entity2.h"
#include "scripts_manager.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#undef min
#undef max
#include <filesystem>
#include "entity.h"
#include "component.h"

scripts_manager* g_all_scripts;
JSRuntime* g_js_runtime;

void register_listener(HANDLE dir_handle, char* const info_buffer, DWORD const info_buffer_size, OVERLAPPED* const overlapped)
{
	BOOL status = ReadDirectoryChangesW(
		dir_handle,
		info_buffer,
		info_buffer_size,
		true,
		FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME,
		NULL,
		overlapped,
		NULL
	);
	if (!status)
	{
		printf("CHANGE LISTENER ERROR\n");
		exit(1);
	}
	printf("listener registered\n");
}

int main(int argc, char* argv[])
{
	entity e;
	e.add_component<script_component>("Hello");
	e.add_component<physics_component>();



	constexpr static char WATCH_BASE_DIR[] = "../../../../cwd/res/scripts/out";
	const std::filesystem::path WATCH_BASE_PATH(WATCH_BASE_DIR);

	HANDLE dir_handle = CreateFile(
		WATCH_BASE_DIR,
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL
	);
	if (dir_handle == INVALID_HANDLE_VALUE)
	{
		printf("HANDLE error\n");
		exit(1);
	}
	constexpr static DWORD INFO_BUFFER_SIZE = 16 * sizeof(FILE_NOTIFY_INFORMATION);
	char info_buffer[INFO_BUFFER_SIZE] = { 0 };
	OVERLAPPED overlapped = { 0 };
	overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (overlapped.hEvent == NULL)
	{
		printf("EVENT ERROR\n");
		exit(1);
	}
	register_listener(dir_handle, info_buffer, INFO_BUFFER_SIZE, &overlapped);

	g_js_runtime = JS_NewRuntime();
	g_all_scripts = new scripts_manager(WATCH_BASE_DIR);
	entity2* const test_e = new entity2("test.js");

	std::string old_fp;
	while (!(GetKeyState('Q') & 0x8000))
	{
		// e.run();

		// non blocking wait
		if (WaitForSingleObject(overlapped.hEvent, 0) == WAIT_OBJECT_0)
		{
			DWORD num_bytes = 0;
			DWORD result = GetOverlappedResult(dir_handle, &overlapped, &num_bytes, FALSE);
			if (!result)
			{
				printf("OVERLAPPED RESULT ERROR\n");
				exit(1);
			}

			DWORD offset = 0;
			FILE_NOTIFY_INFORMATION* cur = nullptr;
			char fp[1024] = { 0 };
			do
			{
				cur = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&info_buffer[offset]);

				memset(fp, 0, sizeof(fp));
				WideCharToMultiByte(CP_ACP, NULL, cur->FileName, cur->FileNameLength / sizeof(WCHAR), fp, sizeof(fp), NULL, NULL);
				printf("%s %d\n", fp, cur->Action);
				if (std::filesystem::is_directory(WATCH_BASE_PATH / fp))
				{
					printf("SKIP directory\n");
					continue;
				}

				switch (cur->Action)
				{
				case FILE_ACTION_MODIFIED:
					g_all_scripts->reload_file(fp);
					break;
				case FILE_ACTION_REMOVED:
				case FILE_ACTION_RENAMED_OLD_NAME:
					old_fp = fp;
					break;
				case FILE_ACTION_ADDED:
				case FILE_ACTION_RENAMED_NEW_NAME:
					g_all_scripts->rename_file(old_fp, fp);
					break;
				}

				offset += cur->NextEntryOffset;
			} while (cur->NextEntryOffset != 0);

			ResetEvent(overlapped.hEvent);
			register_listener(dir_handle, info_buffer, INFO_BUFFER_SIZE, &overlapped);
		}

		test_e->update();
	}
	delete test_e;

	delete g_all_scripts;
	JS_FreeRuntime(g_js_runtime);

	return 0;
}
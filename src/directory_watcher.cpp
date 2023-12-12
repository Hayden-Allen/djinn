#include "pch.h"
#include "directory_watcher.h"


directory_watcher::directory_watcher(std::string const& base_dir) :
	m_base_path(base_dir)
{
	m_dir_handle = CreateFile(
		base_dir.c_str(),
		GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED,
		NULL);
	if (m_dir_handle == INVALID_HANDLE_VALUE)
		err("HANDLE error");

	m_overlapped = { 0 };
	m_overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (m_overlapped.hEvent == NULL)
		err("OVERLAPPED error");

	register_listener();
}
directory_watcher::~directory_watcher()
{
}



void directory_watcher::poll()
{
	if (WaitForSingleObject(m_overlapped.hEvent, 0) == WAIT_OBJECT_0)
	{
		DWORD num_bytes = 0;
		DWORD result = GetOverlappedResult(m_dir_handle, &m_overlapped, &num_bytes, FALSE);
		if (!result)
			err("OVERLAPPED RESULT");

		DWORD offset = 0;
		FILE_NOTIFY_INFORMATION* cur = nullptr;
		char fp[MAX_PATH] = { 0 };
		do
		{
			cur = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&m_info_buffer[offset]);
			memset(fp, 0, sizeof(fp));
			WideCharToMultiByte(CP_ACP, NULL, cur->FileName, cur->FileNameLength / sizeof(WCHAR), fp, sizeof(fp), NULL, NULL);
			if (std::filesystem::is_directory(m_base_path / fp))
				continue;

			handle_action(cur->Action, fp);

			offset += cur->NextEntryOffset;
		} while (cur->NextEntryOffset != 0);

		ResetEvent(m_overlapped.hEvent);
		register_listener();
	}
}
void directory_watcher::register_listener()
{
	BOOL const status = ReadDirectoryChangesW(
		m_dir_handle,
		m_info_buffer,
		s_INFO_BUFFER_SIZE,
		true,
		FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME,
		NULL,
		&m_overlapped,
		NULL);
	if (!status)
		err("REGISTER LISTENER error");
}
void directory_watcher::err(std::string const& msg)
{
	printf("%s\n", msg.c_str());
	exit(1);
}

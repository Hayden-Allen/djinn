#pragma once
#include "pch.h"

namespace djinn
{
	class directory_watcher
	{
	public:
		directory_watcher(std::string const& base_dir);
		DCM(directory_watcher);
		virtual ~directory_watcher();
	public:
		void poll();
		virtual void handle_action(DWORD const action, std::string const& fp) = 0;
	protected:
		void register_listener();
		void err(std::string const& msg);
	protected:
		static constexpr DWORD s_INFO_BUFFER_SIZE = 16 * sizeof(FILE_NOTIFY_INFORMATION);
	protected:
		std::filesystem::path m_base_path;
		HANDLE m_dir_handle;
		OVERLAPPED m_overlapped;
		char m_info_buffer[s_INFO_BUFFER_SIZE] = { 0 };
	};
} // namespace djinn

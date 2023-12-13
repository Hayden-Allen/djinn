#pragma once
#include "pch.h"

namespace djinn
{
	class entity;

	class script_manager final
	{
	public:
		script_manager(std::string const& base_dir);
		DCM(script_manager);
		~script_manager();
	public:
		entity* load(std::string const& fp);
		void remove(entity* const e);
		void reload(std::string const& fp);
		void rename(std::string const& old_fp, std::string const& new_fp);
	private:
		std::string to_absolute(std::string const& fp);
		void inject_script(std::string const& afp);
		void load_file(std::string const& afp);
	private:
		JSRuntime* m_runtime;
		std::filesystem::path m_base_dir;
		std::unordered_map<std::string, std::string> m_fp2src;
		haul::bdumap<entity*, std::string> m_entity2fp;
	};
} // namespace djinn

#pragma once
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <filesystem>
#include "entity2.h"

struct scripts_manager {
public:
	scripts_manager(const std::string& base_dir);
	~scripts_manager();

	void register_entity(entity2* e, const std::string& fn);
	void unregister_entity(entity2* e);
	void reload_file(const std::string& fn, bool const bypass = false);
	void rename_file(const std::string& oldfp, const std::string& newfp);
private:
	void inject_file_to_all_entities(const std::string& afn);
	std::string to_absolute(const std::string& fp);
private:
	std::filesystem::path m_base_dir;
	std::unordered_map<std::string, std::string> fn2src;
	std::unordered_map<entity2*, std::string> e2fn;
	std::unordered_map<std::string, std::unordered_set<entity2*>> fn2e_multi;
};

extern scripts_manager* g_all_scripts;

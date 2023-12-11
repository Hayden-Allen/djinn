#include "entity2.h"
#include "scripts_manager.h"

scripts_manager::scripts_manager(const std::string& base_dir) :
	m_base_dir(base_dir)
{
	//
}

scripts_manager::~scripts_manager() {
	//
}

void scripts_manager::register_entity(entity2* e, const std::string& fn) {
	const std::string& afn = to_absolute(fn);
	e2fn.insert({ e, afn });
	fn2e_multi[afn].insert(e);
	if (fn2src.contains(afn)) {
		inject_file_to_all_entities(afn);
	}
	else {
		reload_file(afn, true);
	}
}

void scripts_manager::unregister_entity(entity2* e) {
	if (e2fn.contains(e)) {
		fn2e_multi.at(e2fn.at(e)).erase(e);
		e2fn.erase(e);
	}
}

void scripts_manager::reload_file(const std::string& fn, bool const bypass) {
	const std::string& afn = to_absolute(fn);
	if (!bypass && !fn2src.contains(afn)) return;

	std::ifstream ifs(afn);
	if (!ifs.is_open()) __debugbreak();
	std::ostringstream sstr;
	sstr << ifs.rdbuf();
	fn2src.insert_or_assign(afn, sstr.str());
	inject_file_to_all_entities(afn);
}

void scripts_manager::rename_file(const std::string& oldfp, const std::string& newfp)
{
	const std::string& oafn = to_absolute(oldfp);
	if (!fn2src.contains(oafn)) return;

	const std::string& nafn = to_absolute(newfp);
	fn2src.insert({ nafn, fn2src.at(oafn) });
	fn2src.erase(oafn);
	for (entity2* const e : fn2e_multi.at(oafn))
	{
		e2fn[e] = nafn;
	}
	fn2e_multi.insert({ nafn, fn2e_multi.at(oafn) });
	fn2e_multi.erase(oafn);
}

void scripts_manager::inject_file_to_all_entities(const std::string& afn) {
	for (entity2* e : fn2e_multi.at(afn)) {
		e->inject_script_src(afn, fn2src.at(afn));
	}
}

std::string scripts_manager::to_absolute(const std::string& fp)
{
	return std::filesystem::absolute(m_base_dir / fp).string();
}

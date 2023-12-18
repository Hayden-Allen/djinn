// #include "pch.h"
// #include "script_manager.h"
// #include "core/util.h"
// #include "entity.h"
// #include "core/constants.h"
//
// namespace djinn
//{
//	script_manager::script_manager() :
//		m_runtime(JS_NewRuntime()),
//		m_base_dir(c::base_dir::script)
//	{}
//	script_manager::~script_manager()
//	{
//		JS_FreeRuntime(m_runtime);
//	}
//
//
//
//	entity* script_manager::load(std::string const& fp)
//	{
//		std::string const& afp = to_absolute(fp);
//		entity* const e = new entity(fp, m_runtime);
//		m_entity2fp.insert(e, afp);
//
//		if (m_fp2src.contains(afp))
//			inject_script(afp);
//		else
//			load_file(afp);
//		return e;
//	}
//	void script_manager::remove(entity* const e)
//	{
//		m_entity2fp.erase_key(e);
//	}
//	void script_manager::reload(std::string const& fp)
//	{
//		std::string const& afp = to_absolute(fp);
//		if (!m_fp2src.contains(afp))
//			return;
//		load_file(afp);
//	}
//	void script_manager::rename(std::string const& old_fp, std::string const& new_fp)
//	{
//		std::string const& old_afp = to_absolute(old_fp);
//		ASSERT(m_fp2src.contains(old_afp));
//		std::string const& new_afp = to_absolute(new_fp);
//		m_fp2src.insert({ new_afp, m_fp2src.at(old_afp) });
//		m_fp2src.erase(old_afp);
//		m_entity2fp.replace_val(old_afp, new_afp);
//	}
//
//
//
//	std::string script_manager::to_absolute(std::string const& fp)
//	{
//		return u::to_absolute(m_base_dir, fp);
//	}
//	void script_manager::inject_script(std::string const& afp)
//	{
//		std::string const& src = m_fp2src.at(afp);
//		for (entity* const e : m_entity2fp.get_key(afp))
//			e->inject_script(afp, src);
//	}
//	void script_manager::load_file(std::string const& afp)
//	{
//		std::ifstream ifs(afp);
//		ASSERT(ifs.is_open());
//
//		std::ostringstream sstr;
//		sstr << ifs.rdbuf();
//
//		m_fp2src.insert_or_assign(afp, sstr.str());
//		inject_script(afp);
//	}
// } // namespace djinn

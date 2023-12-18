#include "pch.h"
#include "entity_manager.h"
#include "scene/entity.h"
#include "scene/camera_entity.h"
#include "core/constants.h"

namespace djinn
{
	entity_manager::entity_manager() :
		manager(c::base_dir::script),
		m_runtime(JS_NewRuntime())
	{}
	entity_manager::~entity_manager()
	{
		JS_FreeRuntime(m_runtime);
	}



	id_t entity_manager::load(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		return load_base(new entity(afp, m_runtime), afp);
	}
	id_t entity_manager::load_camera(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		id_t const id = load_base(new camera_entity(afp, m_runtime), afp);
		m_cameras.insert(id);
		return id;
	}
	void entity_manager::reload(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		if (!m_afp2src.contains(afp))
			return;
		load_file(afp);
	}
	void entity_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		std::string const& old_afp = to_absolute(old_fp);
		if (!m_afp2src.contains(old_afp))
			return;
		std::string const& new_afp = to_absolute(new_fp);
		m_afp2src.insert({ new_afp, m_afp2src.at(old_afp) });
		m_afp2src.erase(old_afp);
		m_id2afp.replace_val(old_afp, new_afp);
	}
	void entity_manager::destroy(id_t const id)
	{
		erase(id);
		m_id2afp.erase_key(id);
		m_cameras.erase(id);
	}
	camera_entity* entity_manager::get_camera(id_t const id)
	{
		ASSERT(m_cameras.contains(id));
		return (camera_entity*)get(id).get();
	}



	void entity_manager::inject_script(std::string const& afp)
	{
		std::string const& src = m_afp2src.at(afp);
		for (id_t const id : m_id2afp.get_key(afp))
			get(id)->inject_script(afp, src);
	}
	void entity_manager::load_file(std::string const& afp)
	{
		m_afp2src.insert_or_assign(afp, u::read_file(afp));
		inject_script(afp);
	}
	id_t entity_manager::load_base(entity* const e, std::string const& afp)
	{
		id_t const id = insert(e);
		m_id2afp.insert(id, afp);
		if (m_afp2src.contains(afp))
			inject_script(afp);
		else
			load_file(afp);
		return id;
	}
} // namespace djinn

#include "pch.h"
#include "shader_manager.h"
#include "core/constants.h"

namespace djinn
{
	shader_manager::shader_manager() :
		manager(c::base_dir::shader)
	{}



	id_t shader_manager::create(std::string const& vert_src, std::string const& frag_src)
	{
		shaders* const s = new shaders(vert_src, frag_src, true);
		return insert(s);
	}
	id_t shader_manager::load(std::string const& fp)
	{
		ASSERT(false);
		return 0;
	}
	void shader_manager::remove(id_t const id)
	{
		ASSERT(false);
	}
	void shader_manager::reload(std::string const& fp)
	{
		ASSERT(false);
	}
	void shader_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		ASSERT(false);
	}
}

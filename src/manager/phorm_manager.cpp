#include "pch.h"
#include "phorm_manager.h"
#include "core/constants.h"

namespace djinn
{
	phorm_manager::phorm_manager() :
		ref_counted_file_manager(c::base_dir::phorm)
	{}



	std::vector<id_t> phorm_manager::load_all(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		mgl::input_file in(afp);

		u64 const ro_count = in.ulong();
		std::vector<id_t> ids;
		ids.reserve(ro_count);
		for (u64 i = 0; i < ro_count; i++)
		{
			phorm* const p = new phorm(s_next_id, &in);
			ids.push_back(insert(p));
		}
		return ids;
	}
	id_t phorm_manager::load(std::string const& fp)
	{
		ASSERT(false);
		return 0;
	}
	void phorm_manager::reload(std::string const& fp)
	{
		ASSERT(false);
	}
	void phorm_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		ASSERT(false);
	}
	void phorm_manager::destroy(id_t const id)
	{
		try_erase(id);
	}
} // namespace djinn

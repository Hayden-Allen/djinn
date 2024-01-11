#include "pch.h"
#include "phorm_manager.h"
#include "core/constants.h"

namespace djinn
{
	phorm_manager::phorm_manager()
	{}



	std::vector<id_t> phorm_manager::load_all(mgl::input_file* const in)
	{
		u64 const ro_count = in->ulong();
		std::vector<id_t> ids;
		ids.reserve(ro_count);
		for (u64 i = 0; i < ro_count; i++)
		{
			phorm* const p = new phorm(s_next_id, in);
			ids.push_back(insert(p));
		}
		return ids;
	}
	void phorm_manager::destroy(id_t const id)
	{
		try_erase(id);
	}
} // namespace djinn

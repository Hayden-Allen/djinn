#include "pch.h"
#include "phorm_manager.h"
#include "core/constants.h"

namespace djinn
{
	phorm_manager::phorm_manager()
	{}



	std::vector<id_t> phorm_manager::load_xport(mgl::input_file* const in, std::unordered_map<u32, sptr<material>> const& mats)
	{
		u64 const count = in->ulong();
		std::vector<id_t> ids;
		ids.reserve(count);
		for (u64 i = 0; i < count; i++)
		{
			phorm* const p = new phorm(s_next_id, in, mats);
			ids.push_back(insert(p));
		}
		return ids;
	}
} // namespace djinn

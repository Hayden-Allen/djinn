#include "pch.h"
#include "phorm_manager.h"
#include "core/constants.h"

namespace djinn
{
	phorm_manager::phorm_manager()
	{}



	std::vector<id_t> phorm_manager::load_xport(mgl::input_file* const in, std::unordered_map<u32, sptr<material>> const& mats)
	{
		std::vector<id_t> ids;

		u64 const sg_count = in->ulong();
		printf("sg: %zu\n", sg_count);
		for (u64 i = 0; i < sg_count; i++)
		{
			printf("\tA %zu\n", in->get_pos());
			phorm* const p = new phorm(s_next_id, in, mats);
			printf("\tB %zu\n", in->get_pos());
			ids.push_back(insert(p));
			printf("\tC %zu\n", in->get_pos());
		}
		printf("\tD %zu\n", in->get_pos());

		in->check_error();
		printf("\tE %zu\n", in->get_pos());
		u64 const sm_count = in->ulong();
		in->check_error();
		printf("\tF %zu\n", in->get_pos());
		printf("sm: %zu\n", sm_count);
		for (u64 i = 0; i < sm_count; i++)
		{
			phorm* const p = new phorm(s_next_id, in, mats);
			ids.push_back(insert(p));
		}

		return ids;
	}
} // namespace djinn

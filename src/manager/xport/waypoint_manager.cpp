#include "pch.h"
#include "waypoint_manager.h"

namespace djinn
{
	waypoint_manager::waypoint_manager()
	{}



	std::vector<id_t> waypoint_manager::load_xport(mgl::input_file* const in)
	{
		u64 const count = in->ulong();
		printf("w: %zu\n", count);
		std::vector<id_t> ids;
		ids.reserve(count);
		for (u64 i = 0; i < count; i++)
		{
			waypoint* const w = new waypoint(s_next_id, in);
			ids.push_back(insert(w));
		}
		return ids;
	}
	id_t waypoint_manager::create()
	{
		return insert(new waypoint(s_next_id));
	}
	void waypoint_manager::destroy(id_t const id)
	{
		manager::destroy(id);
	}
} // namespace djinn

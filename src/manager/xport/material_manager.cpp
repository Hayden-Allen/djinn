#include "pch.h"
#include "material_manager.h"

namespace djinn
{
	material_manager::material_manager()
	{}



	std::unordered_map<u32, sptr<material>> material_manager::load_xport(mgl::input_file* const in, std::vector<sptr<texture>> const& tex)
	{
		u64 const count = in->ulong();
		printf("m: %zu\n", count);
		std::unordered_map<u32, sptr<material>> ret;
		ret.reserve(count);
		for (u64 i = 0; i < count; i++)
		{
			u32 const idx = in->uint();
			id_t const id = insert(new material(in, tex));
			ret.insert({ idx, get(id) });
		}
		return ret;
	}
} // namespace djinn

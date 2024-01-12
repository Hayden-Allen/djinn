#include "pch.h"
#include "material_manager.h"

namespace djinn
{
	material_manager::material_manager()
	{}



	std::vector<id_t> material_manager::load_all(mgl::input_file* const in)
	{
		ASSERT(false);
		return {};
	}
	std::unordered_map<u32, sptr<material>> material_manager::load_all(mgl::input_file* const in, std::vector<id_t> const& tex_ids)
	{
		u64 const num_materials = in->ulong();
		std::unordered_map<u32, sptr<material>> ret;
		ret.reserve(num_materials);
		for (u64 i = 0; i < num_materials; i++)
		{
			u32 const idx = in->uint();
			id_t const id = insert(new material(in, tex_ids));
			ret.insert({ idx, get(id) });
		}
		return ret;
	}
} // namespace djinn

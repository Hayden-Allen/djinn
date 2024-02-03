#include "pch.h"
#include "phorm_manager.h"
#include "core/constants.h"
#include "script/service/render_service.h"

namespace djinn
{
	phorm_manager::phorm_manager()
	{}



	std::vector<id_t> phorm_manager::load_xport(mgl::input_file* const in, std::unordered_map<u32, sptr<material>> const& mats)
	{
		std::vector<id_t> ids;

		u64 const sg_count = in->ulong();
		printf("sg: %zu\n", sg_count);
		insert_from_file(in, sg_count, mats, &ids);

		u64 const sm_count = in->ulong();
		printf("sm: %zu\n", sm_count);
		insert_from_file(in, sm_count, mats, &ids);

		return ids;
	}
	void phorm_manager::set_layer(id_t const id, u32 const layer)
	{
		ASSERT(m_id2layer.contains(id));
		u32 const current = m_id2layer.at(id);
		m_layer2ids.at(current).erase(id);
		m_layer2ids[layer].insert(id);
	}
	void phorm_manager::draw_all()
	{
		for (auto const& layer : m_layer2ids)
			for (id_t const id : layer.second)
				get(id)->draw(render_service::get_context());
	}



	void phorm_manager::insert_from_file(mgl::input_file* const in, u64 const count, std::unordered_map<u32, sptr<material>> const& mats, std::vector<id_t>* const out_ids)
	{
		for (u64 i = 0; i < count; i++)
		{
			phorm* const p = new phorm(s_next_id, in, mats);
			id_t const id = insert(p);
			out_ids->push_back(id);
			m_layer2ids[0].insert(id);
			m_id2layer.insert({ id, 0 });
		}
	}
} // namespace djinn

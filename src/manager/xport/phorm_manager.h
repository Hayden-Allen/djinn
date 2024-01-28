#pragma once
#include "pch.h"
#include "manager/manager.h"
#include "scene/xport/phorm.h"

namespace djinn
{
	class phorm_manager final : public ref_counted_manager<phorm>
	{
	public:
		phorm_manager();
		DCM(phorm_manager);
	public:
		std::vector<id_t> load_xport(mgl::input_file* const in, std::unordered_map<u32, sptr<material>> const& mats);
		void set_layer(id_t const id, u32 const layer);
		void draw_all();
	private:
		std::map<u32, std::unordered_set<id_t>> m_layer2ids;
		std::unordered_map<id_t, u32> m_id2layer;
	private:
		void insert_from_file(mgl::input_file* const in, u64 const count, std::unordered_map<u32, sptr<material>> const& mats, std::vector<id_t>* const out_ids);
	};
} // namespace djinn

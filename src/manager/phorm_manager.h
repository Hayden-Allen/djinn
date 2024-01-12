#pragma once
#include "pch.h"
#include "manager.h"
#include "power_loader.h"
#include "scene/phorm.h"

namespace djinn
{
	class phorm_manager final : public ref_counted_manager<phorm>, public power_loader
	{
	public:
		phorm_manager();
		DCM(phorm_manager);
	public:
		std::vector<id_t> load_all(mgl::input_file* const in) override;
		std::vector<id_t> load_all(mgl::input_file* const in, std::unordered_map<u32, sptr<material>> const& mats);
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "manager.h"
#include "power_loader.h"
#include "asset/material.h"

namespace djinn
{
	class material_manager final : public manager<material>, public power_loader
	{
	public:
		material_manager();
		DCM(material_manager);
	public:
		std::vector<id_t> load_all(mgl::input_file* const in) override;
		std::unordered_map<u32, sptr<material>> load_all(mgl::input_file* const in, std::vector<id_t> const& tex_ids);
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "manager/manager.h"
#include "asset/xport/material.h"

namespace djinn
{
	class material_manager final : public manager<material>
	{
	public:
		material_manager();
		DCM(material_manager);
	public:
		std::unordered_map<u32, sptr<material>> load_xport(mgl::input_file* const in, std::vector<sptr<texture>> const& tex);
	};
} // namespace djinn

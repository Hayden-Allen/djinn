#pragma once
#include "pch.h"
#include "manager.h"
#include "power_loader.h"
#include "scene/waypoint.h"

namespace djinn
{
	class waypoint_manager final : public ref_counted_manager<waypoint>, public power_loader
	{
	public:
		waypoint_manager();
		DCM(waypoint_manager);
	public:
		std::vector<id_t> load_all(mgl::input_file* const in) override;
		id_t create();
		void destroy(id_t const id) override;
	};
} // namespace djinn

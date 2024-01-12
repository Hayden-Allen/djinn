#pragma once
#include "pch.h"
#include "manager/manager.h"
#include "scene/xport/waypoint.h"

namespace djinn
{
	class waypoint_manager final : public ref_counted_manager<waypoint>
	{
	public:
		waypoint_manager();
		DCM(waypoint_manager);
	public:
		std::vector<id_t> load_xport(mgl::input_file* const in);
		id_t create();
		void destroy(id_t const id) override;
	};
} // namespace djinn

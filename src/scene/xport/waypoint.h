#pragma once
#include "pch.h"
#include "scene/scene_object_base.h"
#include "xport.h"

namespace djinn
{
	class waypoint : public scene_object_base, public xport
	{
	public:
		waypoint(id_t const id);
		waypoint(id_t const id, mgl::input_file* const in);
		DCM(waypoint);
	};
} // namespace djinn

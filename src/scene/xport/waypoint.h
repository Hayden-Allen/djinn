#pragma once
#include "pch.h"
#include "scene/scene_object.h"
#include "xport.h"

namespace djinn
{
	class waypoint : public scene_object, public xport
	{
	public:
		waypoint(id_t const id);
		waypoint(id_t const id, mgl::input_file* const in);
		DCM(waypoint);
	};
} // namespace djinn

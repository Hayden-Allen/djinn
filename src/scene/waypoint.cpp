#include "pch.h"
#include "waypoint.h"

namespace djinn
{
	waypoint::waypoint(id_t const id) :
		scene_object(id)
	{}
	waypoint::waypoint(id_t const id, mgl::input_file* const in) :
		scene_object(id),
		xport(in)
	{
		in->read(m_transform.e, 16);
		extract_transform(m_transform);
	}
} // namespace djinn

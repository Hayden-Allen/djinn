#include "pch.h"
#include "visibility_scene_object.h"

namespace djinn
{
	visibility_scene_object::~visibility_scene_object()
	{}



	tmat<space::OBJECT, space::WORLD> visibility_scene_object::get_graphics_transform() const
	{
		if (!m_visible)
		{
			f32 const zeros[16] = { 0 };
			return tmat<space::OBJECT, space::WORLD>(zeros);
		}
		return get_world_transform();
	}
	void visibility_scene_object::set_visible(bool const visible)
	{
		m_visible = visible;
	}



	visibility_scene_object::visibility_scene_object(id_t const id) :
		scene_object(id),
		m_visible(true)
	{}
} // namespace djinn

#include "pch.h"
#include "light.h"

namespace djinn
{
	light::light(id_t const id) :
		visibility_scene_object(id)
	{}



	void light::update_transform()
	{
		visibility_scene_object::update_transform();
		tmat<space::OBJECT, space::WORLD> const& mat = get_world_transform();
		m_raw.mat = mat;
		m_raw.inv_mat = mat.invert_copy();
	}
	mgl::light const& light::get_raw() const
	{
		return m_raw;
	}
} // namespace djinn

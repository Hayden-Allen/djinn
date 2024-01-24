#include "pch.h"
#include "light.h"

namespace djinn
{
	light::light(id_t const id) :
		visibility_scene_object(id),
		m_type(light_type::NONE)
	{}

	light::light(id_t const id, mgl::input_file* const in) :
		visibility_scene_object(id),
		xport(in)
	{
		m_raw.load(in);
		ASSERT(m_raw.mat.t[3] == m_raw.inv_mat.t[3]);
		m_type = (light_type)m_raw.mat.t[3];
		// copy to scene_object::m_transform
		tmat<space::OBJECT, space::WORLD> o2w = m_raw.mat;
		o2w.t[3] = 1.f;
		ASSERT(!m_parent);
		m_transform = o2w.cast_copy<space::OBJECT, space::PARENT>();
	}
	tmat<space::OBJECT, space::WORLD> light::get_graphics_transform() const
	{
		return m_raw.mat;
	}



	void light::update_transform()
	{
		tmat<space::OBJECT, space::WORLD> const& mat = get_world_transform();
		m_raw.mat = mat;
		m_raw.inv_mat = mat.invert_copy();
		m_raw.mat.t[3] = m_raw.inv_mat.t[3] = (f32)m_type;
	}
	mgl::light const& light::get_raw() const
	{
		return m_raw;
	}
	mgl::light& light::get_raw()
	{
		return m_raw;
	}
} // namespace djinn

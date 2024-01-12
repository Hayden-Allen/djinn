#include "pch.h"
#include "light.h"

namespace djinn
{
	light::light(id_t const id) :
		visibility_scene_object(id)
	{}

	light::light(id_t const id, mgl::input_file* const in) :
		visibility_scene_object(id),
		xport(in)
	{
		m_raw.load(in);
		// TODO multiply by accumulated parent inverse
		extract_transform(m_raw.mat.cast_copy<space::OBJECT, space::PARENT>());
	}



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
	mgl::light& light::get_raw()
	{
		return m_raw;
	}
} // namespace djinn

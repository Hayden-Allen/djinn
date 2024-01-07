#include "pch.h"
#include "scene_object.h"

namespace djinn
{
	scene_object::~scene_object()
	{}



	void scene_object::update_transform()
	{
		tmat<space::OBJECT, space::PARENT> const& trans = tmat_util::translation<space::OBJECT, space::PARENT>(m_pos[0], m_pos[1], m_pos[2]);
		tmat<space::OBJECT, space::OBJECT> const& rot = tmat_util::rotation_yxz<space::OBJECT>(m_rot[0], m_rot[1], m_rot[2]);
		tmat<space::OBJECT, space::OBJECT> const& scale = tmat_util::scale<space::OBJECT>(m_scale[0], m_scale[1], m_scale[2]);
		m_transform = trans * rot * scale;
	}
	tmat<space::OBJECT, space::PARENT> scene_object::get_transform() const
	{
		return m_transform;
	}
	tmat<space::OBJECT, space::WORLD> scene_object::get_world_transform() const
	{
		// TODO return m_transform * accumulate_parent_mats();
		return m_transform.cast_copy<space::OBJECT, space::WORLD>();
	}



	scene_object::scene_object(id_t const id) :
		scene_object_base(id)
	{}
} // namespace djinn

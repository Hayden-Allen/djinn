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
	void scene_object::set_transform(tmat<space::OBJECT, space::PARENT> const& transform)
	{
		m_transform = transform;
	}
	void scene_object::set_world_transform(tmat<space::OBJECT, space::WORLD> const& transform)
	{
		// TODO
		// tmat<space::PARENT, space::WORLD> const& parent = accumulate_parent_mats();
		// m_transform = transform * parent.invert_copy();
		m_transform = transform.cast_copy<space::OBJECT, space::PARENT>();
	}
	tmat<space::OBJECT, space::PARENT> const& scene_object::get_transform() const
	{
		return m_transform;
	}
	tmat<space::OBJECT, space::WORLD> scene_object::get_world_transform() const
	{
		return m_transform.cast_copy<space::OBJECT, space::WORLD>();
	}
	f32 const* scene_object::get_pos() const
	{
		return m_pos;
	}
	void scene_object::set_pos(f32 const x, f32 const y, f32 const z)
	{
		m_pos[0] = x;
		m_pos[1] = y;
		m_pos[2] = z;
	}
	void scene_object::set_pos_x(f32 const x)
	{
		m_pos[0] = x;
	}
	void scene_object::set_pos_y(f32 const y)
	{
		m_pos[1] = y;
	}
	void scene_object::set_pos_z(f32 const z)
	{
		m_pos[2] = z;
	}
	void scene_object::add_pos(f32 const x, f32 const y, f32 const z)
	{
		m_pos[0] += x;
		m_pos[1] += y;
		m_pos[2] += z;
	}
	void scene_object::add_pos_local(f32 const x, f32 const y, f32 const z)
	{
		vec<space::OBJECT> const local(x, y, z);
		vec<space::PARENT> const parent = m_transform * local;
		m_pos[0] += parent.x;
		m_pos[1] += parent.y;
		m_pos[2] += parent.z;
	}
	f32 const* scene_object::get_rot() const
	{
		return m_rot;
	}
	void scene_object::set_rot(f32 const x, f32 const y, f32 const z)
	{
		m_rot[0] = x;
		m_rot[1] = y;
		m_rot[2] = z;
	}
	void scene_object::set_rot_x(f32 const x)
	{
		m_rot[0] = x;
	}
	void scene_object::set_rot_y(f32 const y)
	{
		m_rot[1] = y;
	}
	void scene_object::set_rot_z(f32 const z)
	{
		m_rot[2] = z;
	}
	void scene_object::add_rot(f32 const x, f32 const y, f32 const z)
	{
		m_rot[0] += x;
		m_rot[1] += y;
		m_rot[2] += z;
	}
	void scene_object::add_rot_local(f32 const x, f32 const y, f32 const z)
	{
		vec<space::OBJECT> const local(x, y, z);
		vec<space::PARENT> const parent = m_transform * local;
		m_rot[0] += parent.x;
		m_rot[1] += parent.y;
		m_rot[2] += parent.z;
	}
	f32 const* scene_object::get_scale() const
	{
		return m_scale;
	}
	void scene_object::set_scale(f32 const x, f32 const y, f32 const z)
	{
		ASSERT(x != 0 && y != 0 && z != 0)
		m_scale[0] = x;
		m_scale[1] = y;
		m_scale[2] = z;
	}
	void scene_object::set_scale_x(f32 const x)
	{
		ASSERT(x != 0)
		m_scale[0] = x;
	}
	void scene_object::set_scale_y(f32 const y)
	{
		ASSERT(y != 0)
		m_scale[1] = y;
	}
	void scene_object::set_scale_z(f32 const z)
	{
		ASSERT(z != 0)
		m_scale[2] = z;
	}
	void scene_object::add_scale(f32 const x, f32 const y, f32 const z)
	{
		m_scale[0] += x;
		m_scale[1] += y;
		m_scale[2] += z;
		ASSERT(m_scale[0] != 0 && m_scale[1] != 0 && m_scale[2] != 0)
	}
	void scene_object::add_scale_local(f32 const x, f32 const y, f32 const z)
	{
		vec<space::OBJECT> const local(x, y, z);
		vec<space::PARENT> const parent = m_transform * local;
		m_rot[0] += parent.x;
		m_rot[1] += parent.y;
		m_rot[2] += parent.z;
		ASSERT(m_scale[0] != 0 && m_scale[1] != 0 && m_scale[2] != 0)
	}



	scene_object::scene_object(id_t const id) :
		identifiable(id)
	{}
} // namespace djinn
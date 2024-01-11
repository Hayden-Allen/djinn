#include "pch.h"
#include "scene_object_base.h"

namespace djinn
{
	scene_object_base::~scene_object_base()
	{}



	tmat<space::OBJECT, space::WORLD> scene_object_base::get_graphics_transform() const
	{
		return get_world_transform();
	}
	void scene_object_base::copy_transform(sptr<scene_object_base> const& other)
	{
		m_pos[0] = other->m_pos[0];
		m_pos[1] = other->m_pos[1];
		m_pos[2] = other->m_pos[2];
		m_rot[0] = other->m_rot[0];
		m_rot[1] = other->m_rot[1];
		m_rot[2] = other->m_rot[2];
		m_scale[0] = other->m_scale[0];
		m_scale[1] = other->m_scale[1];
		m_scale[2] = other->m_scale[2];
	}
	f32 const* scene_object_base::get_pos() const
	{
		return m_pos;
	}
	void scene_object_base::set_pos(f32 const x, f32 const y, f32 const z)
	{
		m_pos[0] = x;
		m_pos[1] = y;
		m_pos[2] = z;
	}
	void scene_object_base::set_pos_x(f32 const x)
	{
		m_pos[0] = x;
	}
	void scene_object_base::set_pos_y(f32 const y)
	{
		m_pos[1] = y;
	}
	void scene_object_base::set_pos_z(f32 const z)
	{
		m_pos[2] = z;
	}
	void scene_object_base::add_pos(f32 const x, f32 const y, f32 const z)
	{
		m_pos[0] += x;
		m_pos[1] += y;
		m_pos[2] += z;
	}
	void scene_object_base::add_pos_local(f32 const x, f32 const y, f32 const z)
	{
		vec<space::OBJECT> const local(x, y, z);
		vec<space::PARENT> const parent = get_transform() * local;
		m_pos[0] += parent.x;
		m_pos[1] += parent.y;
		m_pos[2] += parent.z;
	}
	f32 const* scene_object_base::get_rot() const
	{
		return m_rot;
	}
	void scene_object_base::set_rot(f32 const x, f32 const y, f32 const z)
	{
		m_rot[0] = x;
		m_rot[1] = y;
		m_rot[2] = z;
	}
	void scene_object_base::set_rot_x(f32 const x)
	{
		m_rot[0] = x;
	}
	void scene_object_base::set_rot_y(f32 const y)
	{
		m_rot[1] = y;
	}
	void scene_object_base::set_rot_z(f32 const z)
	{
		m_rot[2] = z;
	}
	void scene_object_base::add_rot(f32 const x, f32 const y, f32 const z)
	{
		m_rot[0] += x;
		m_rot[1] += y;
		m_rot[2] += z;
	}
	void scene_object_base::add_rot_local(f32 const x, f32 const y, f32 const z)
	{
		vec<space::OBJECT> const local(x, y, z);
		vec<space::PARENT> const parent = get_transform() * local;
		m_rot[0] += parent.x;
		m_rot[1] += parent.y;
		m_rot[2] += parent.z;
	}
	f32 const* scene_object_base::get_scale() const
	{
		return m_scale;
	}
	void scene_object_base::set_scale(f32 const x, f32 const y, f32 const z)
	{
		ASSERT(x != 0 && y != 0 && z != 0)
		m_scale[0] = x;
		m_scale[1] = y;
		m_scale[2] = z;
	}
	void scene_object_base::set_scale_x(f32 const x)
	{
		ASSERT(x != 0)
		m_scale[0] = x;
	}
	void scene_object_base::set_scale_y(f32 const y)
	{
		ASSERT(y != 0)
		m_scale[1] = y;
	}
	void scene_object_base::set_scale_z(f32 const z)
	{
		ASSERT(z != 0)
		m_scale[2] = z;
	}
	void scene_object_base::add_scale(f32 const x, f32 const y, f32 const z)
	{
		m_scale[0] += x;
		m_scale[1] += y;
		m_scale[2] += z;
		ASSERT(m_scale[0] != 0 && m_scale[1] != 0 && m_scale[2] != 0)
	}
	void scene_object_base::add_scale_local(f32 const x, f32 const y, f32 const z)
	{
		vec<space::OBJECT> const local(x, y, z);
		vec<space::PARENT> const parent = get_transform() * local;
		m_rot[0] += parent.x;
		m_rot[1] += parent.y;
		m_rot[2] += parent.z;
		ASSERT(m_scale[0] != 0 && m_scale[1] != 0 && m_scale[2] != 0)
	}



	scene_object_base::scene_object_base(id_t const id) :
		identifiable(id)
	{}



	void scene_object_base::extract_transform(tmat<space::OBJECT, space::PARENT> const& mat)
	{
		m_pos[0] = mat.t[0];
		m_pos[1] = mat.t[1];
		m_pos[2] = mat.t[2];
		m_scale[0] = mat.get_i().length();
		m_scale[1] = mat.get_j().length();
		m_scale[2] = mat.get_k().length();

		m_rot[0] = asin(mat.m[2][1]);
		if (abs(mat.m[2][1]) < 0.9999999)
		{
			m_rot[1] = atan2(mat.m[0][2], mat.m[2][2]);
			m_rot[2] = atan2(mat.m[0][1], mat.m[1][1]);
		}
		else
		{
			m_rot[1] = atan2(-mat.m[0][2], mat.m[0][0]);
			m_rot[2] = 0;
		}
	}
} // namespace djinn
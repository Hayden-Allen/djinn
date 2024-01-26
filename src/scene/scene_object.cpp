#include "pch.h"
#include "scene_object.h"
#include "core/util.h"

namespace djinn
{
	scene_object::~scene_object()
	{}



	void scene_object::set_parent(scene_object* const parent)
	{
		m_parent = parent;
	}
	scene_object const* scene_object::get_parent() const
	{
		return m_parent;
	}
	tmat<space::OBJECT, space::PARENT> scene_object::get_transform() const
	{
		return m_transform;
	}
	tmat<space::PARENT, space::WORLD> scene_object::get_parent_transform() const
	{
		if (!m_parent)
			return tmat<space::PARENT, space::WORLD>();
		return m_parent->get_world_transform().cast_copy<space::PARENT, space::WORLD>();
	}
	tmat<space::OBJECT, space::WORLD> scene_object::get_world_transform() const
	{
		return get_parent_transform() * m_transform;
	}
	tmat<space::OBJECT, space::WORLD> scene_object::get_graphics_transform() const
	{
		return get_world_transform();
	}
	void scene_object::copy_transform(sptr<scene_object> const& other)
	{
		m_transform = other->m_transform;
	}
	point<space::PARENT> scene_object::get_pos() const
	{
		return m_transform.get_t();
	}
	std::array<f32, 3> scene_object::get_rot() const
	{
		return { u::rad2deg(m_rot[0]), u::rad2deg(m_rot[1]), u::rad2deg(m_rot[2]) };
	}
	vec<space::PARENT> scene_object::get_scale() const
	{
		f32 const x = m_transform.get_i().length();
		f32 const y = m_transform.get_j().length();
		f32 const z = m_transform.get_k().length();
		return vec<space::PARENT>(x, y, z);
	}
	point<space::WORLD> scene_object::get_pos_world() const
	{
		return get_world_transform().get_t();
	}
	std::array<f32, 3> scene_object::get_rot_world() const
	{
		f32 x = 0, y = 0, z = 0;
		u::extract_rot(get_world_transform(), &x, &y, &z);
		return { u::rad2deg(x), u::rad2deg(y), u::rad2deg(z) };
	}
	vec<space::WORLD> scene_object::get_scale_world() const
	{
		auto const& wt = get_world_transform();
		f32 const x = wt.get_i().length();
		f32 const y = wt.get_j().length();
		f32 const z = wt.get_k().length();
		return vec<space::WORLD>(x, y, z);
	}
	void scene_object::set_pos(point<space::PARENT> const& pos)
	{
		m_transform.t[0] = pos.x;
		m_transform.t[1] = pos.y;
		m_transform.t[2] = pos.z;
	}
	void scene_object::set_pos_x(f32 const x)
	{
		m_transform.t[0] = x;
	}
	void scene_object::set_pos_y(f32 const y)
	{
		m_transform.t[1] = y;
	}
	void scene_object::set_pos_z(f32 const z)
	{
		m_transform.t[2] = z;
	}
	void scene_object::set_rot(f32 const x_deg, f32 const y_deg, f32 const z_deg)
	{
		m_rot[0] = u::deg2rad(x_deg);
		m_rot[1] = u::deg2rad(y_deg);
		m_rot[2] = u::deg2rad(z_deg);
		update_rot();
	}
	void scene_object::set_rot_x(f32 const x_deg)
	{
		m_rot[0] = u::deg2rad(x_deg);
		update_rot();
	}
	void scene_object::set_rot_y(f32 const y_deg)
	{
		m_rot[1] = u::deg2rad(y_deg);
		update_rot();
	}
	void scene_object::set_rot_z(f32 const z_deg)
	{
		m_rot[2] = u::deg2rad(z_deg);
		update_rot();
	}
	void scene_object::set_scale(vec<space::PARENT> const& scale)
	{
		f32 const cx = m_transform.get_i().length();
		f32 const cy = m_transform.get_j().length();
		f32 const cz = m_transform.get_k().length();
		tmat<space::OBJECT, space::OBJECT> const& mat = tmat_util::scale<space::OBJECT>(scale.x / cx, scale.y / cy, scale.z / cz);
		m_transform *= mat;
	}
	void scene_object::set_scale_x(f32 const x)
	{
		f32 const cx = m_transform.get_i().length();
		tmat<space::OBJECT, space::OBJECT> const& mat = tmat_util::scale<space::OBJECT>(x / cx, 1, 1);
		m_transform *= mat;
	}
	void scene_object::set_scale_y(f32 const y)
	{
		f32 const cy = m_transform.get_j().length();
		tmat<space::OBJECT, space::OBJECT> const& mat = tmat_util::scale<space::OBJECT>(1, y / cy, 1);
		m_transform *= mat;
	}
	void scene_object::set_scale_z(f32 const z)
	{
		f32 const cz = m_transform.get_k().length();
		tmat<space::OBJECT, space::OBJECT> const& mat = tmat_util::scale<space::OBJECT>(1, 1, z / cz);
		m_transform *= mat;
	}
	void scene_object::set_pos_world(point<space::WORLD> const& pos)
	{
		point<space::PARENT> const& local_pos = get_parent_transform().invert_copy() * pos;
		set_pos(local_pos);
	}
	void scene_object::set_pos_x_world(f32 const x)
	{
		point<space::WORLD> pos = get_pos_world();
		pos.x = x;
		set_pos_world(pos);
	}
	void scene_object::set_pos_y_world(f32 const y)
	{
		point<space::WORLD> pos = get_pos_world();
		pos.y = y;
		set_pos_world(pos);
	}
	void scene_object::set_pos_z_world(f32 const z)
	{
		point<space::WORLD> pos = get_pos_world();
		pos.z = z;
		set_pos_world(pos);
	}
	void scene_object::set_rot_world(f32 const x_deg, f32 const y_deg, f32 const z_deg)
	{
		ASSERT(false);
	}
	void scene_object::set_rot_x_world(f32 const x_deg)
	{
		ASSERT(false);
	}
	void scene_object::set_rot_y_world(f32 const y_deg)
	{
		ASSERT(false);
	}
	void scene_object::set_rot_z_world(f32 const z_deg)
	{
		ASSERT(false);
	}
	void scene_object::set_scale_world(vec<space::WORLD> const& scale)
	{
		ASSERT(false);
	}
	void scene_object::set_scale_x_world(f32 const x)
	{
		ASSERT(false);
	}
	void scene_object::set_scale_y_world(f32 const y)
	{
		ASSERT(false);
	}
	void scene_object::set_scale_z_world(f32 const z)
	{
		ASSERT(false);
	}
	void scene_object::add_pos(vec<space::PARENT> const& pos)
	{
		set_pos(get_pos() + pos);
	}
	void scene_object::add_pos_x(f32 const x)
	{
		set_pos_x(get_pos().x + x);
	}
	void scene_object::add_pos_y(f32 const y)
	{
		set_pos_y(get_pos().y + y);
	}
	void scene_object::add_pos_z(f32 const z)
	{
		set_pos_z(get_pos().z + z);
	}
	void scene_object::add_rot(f32 const x_deg, f32 const y_deg, f32 const z_deg)
	{
		m_rot[0] += u::deg2rad(x_deg);
		m_rot[1] += u::deg2rad(y_deg);
		m_rot[2] += u::deg2rad(z_deg);
		update_rot();
	}
	void scene_object::add_rot_x(f32 const x_deg)
	{
		m_rot[0] += u::deg2rad(x_deg);
		update_rot();
	}
	void scene_object::add_rot_y(f32 const y_deg)
	{
		m_rot[1] += u::deg2rad(y_deg);
		update_rot();
	}
	void scene_object::add_rot_z(f32 const z_deg)
	{
		m_rot[2] += u::deg2rad(z_deg);
		update_rot();
	}
	void scene_object::add_scale(vec<space::PARENT> const& scale)
	{
		set_scale(get_scale() + scale);
	}
	void scene_object::add_scale_x(f32 const x)
	{
		set_scale_x(get_scale().x + x);
	}
	void scene_object::add_scale_y(f32 const y)
	{
		set_scale_y(get_scale().y + y);
	}
	void scene_object::add_scale_z(f32 const z)
	{
		set_scale_z(get_scale().z + z);
	}
	void scene_object::add_pos_world(vec<space::WORLD> const& pos)
	{
		set_pos_world(get_pos_world() + pos);
	}
	void scene_object::add_pos_x_world(f32 const x)
	{
		set_pos_x_world(get_pos_world().x + x);
	}
	void scene_object::add_pos_y_world(f32 const y)
	{
		set_pos_y_world(get_pos_world().y + y);
	}
	void scene_object::add_pos_z_world(f32 const z)
	{
		set_pos_z_world(get_pos_world().z + z);
	}
	void scene_object::add_rot_world(f32 const x_deg, f32 const y_deg, f32 const z_deg)
	{
		ASSERT(false);
	}
	void scene_object::add_rot_x_world(f32 const x_deg)
	{
		ASSERT(false);
	}
	void scene_object::add_rot_y_world(f32 const y_deg)
	{
		ASSERT(false);
	}
	void scene_object::add_rot_z_world(f32 const z_deg)
	{
		ASSERT(false);
	}
	void scene_object::add_scale_world(vec<space::WORLD> const& scale)
	{
		set_scale_world(get_scale_world() + scale);
	}
	void scene_object::add_scale_x_world(f32 const x)
	{
		set_scale_x_world(get_scale_world().x + x);
	}
	void scene_object::add_scale_y_world(f32 const y)
	{
		set_scale_y_world(get_scale_world().y + y);
	}
	void scene_object::add_scale_z_world(f32 const z)
	{
		set_scale_z_world(get_scale_world().z + z);
	}



	scene_object::scene_object(id_t const id) :
		identifiable(id),
		m_parent(nullptr)
	{}



	void scene_object::update_rot()
	{
		point<space::PARENT> const& t = m_transform.get_t();
		tmat<space::OBJECT, space::PARENT> const& trans = tmat_util::translation<space::OBJECT, space::PARENT>(t.x, t.y, t.z);
		tmat<space::OBJECT, space::OBJECT> const& rot = tmat_util::rotation_yxz<space::OBJECT>(m_rot[0], m_rot[1], m_rot[2]);
		m_transform = trans * rot;
	}
} // namespace djinn
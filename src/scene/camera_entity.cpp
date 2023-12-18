#include "pch.h"
#include "camera_entity.h"

namespace djinn
{
	camera_entity::camera_entity(std::string const& fp, JSRuntime* const runtime) :
		entity(fp, runtime),
		m_proj(pmat_util::projection(45, 1, .01, 1000)),
		m_view(m_transform.invert_cast_copy<space::WORLD, space::CAMERA>()),
		m_vp(m_proj * m_view)
	{}



	void camera_entity::configure(f32 const fov_y, f32 const aspect, f32 const near, f32 const far)
	{
		m_proj = pmat_util::projection(fov_y, aspect, near, far);
		m_vp = m_proj * m_view;
	}
	pmat<space::CAMERA, space::CLIP> const& camera_entity::get_proj() const
	{
		return m_proj;
	}
	tmat<space::WORLD, space::CAMERA> const& camera_entity::get_view() const
	{
		return m_view;
	}
	mat<space::WORLD, space::CLIP> const& camera_entity::get_view_proj() const
	{
		return m_vp;
	}
} // namespace djinn

#include "pch.h"
#include "camera_entity.h"

namespace djinn
{
	camera_entity::camera_entity(id_t const id, std::string const& fp, JSRuntime* const runtime) :
		entity(id, fp, runtime)
	{}



	void camera_entity::configure(f32 const fov_y, f32 const aspect, f32 const near, f32 const far)
	{
		m_cam.configure(fov_y, aspect, near, far);
	}
	pmat<space::CAMERA, space::CLIP> const& camera_entity::get_proj() const
	{
		return m_cam.get_proj();
	}
	tmat<space::WORLD, space::CAMERA> const& camera_entity::get_view() const
	{
		return m_cam.get_view();
	}
	mat<space::WORLD, space::CLIP> const& camera_entity::get_view_proj() const
	{
		return m_cam.get_view_proj();
	}
	void camera_entity::move(f32 const dt, vec<space::CAMERA> const& amount, f32 const mx, f32 const my)
	{
		m_cam.move(dt, amount, mx, my);
	}
} // namespace djinn

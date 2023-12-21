#include "pch.h"
#include "camera_entity.h"

namespace djinn
{
	camera_entity::camera_entity(id_t const id, std::string const& fp, JSRuntime* const runtime) :
		entity(id, fp, runtime),
		m_proj(pmat_util::projection(45, 1, .01, 1000)),
		m_view(m_transform.invert_cast_copy<space::WORLD, space::CAMERA>()),
		m_view_proj(m_proj * m_view),
		m_view_proj_basis(m_proj * m_view.basis_copy())
	{}



	void camera_entity::configure(f32 const fov_y, f32 const aspect, f32 const near, f32 const far)
	{
		// m_cam.configure(fov_y, aspect, near, far);
		m_proj = pmat_util::projection(fov_y, aspect, near, far);
		m_view_proj = m_proj * m_view;
		m_view_proj_basis = m_proj * m_view.basis_copy();
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
		return m_view_proj;
	}
	void camera_entity::multiply_transform(tmat<space::OBJECT, space::OBJECT> const& mat)
	{
		ASSERT(false);
	}
	void camera_entity::multiply_transform(tmat<space::CAMERA, space::CAMERA> const& mat)
	{
		m_transform *= mat.cast_copy<space::OBJECT, space::OBJECT>();
		m_view = m_transform.invert_cast_copy<space::WORLD, space::CAMERA>();
		m_view_proj = m_proj * m_view;
		m_view_proj_basis = m_proj * m_view.basis_copy();
	}
} // namespace djinn

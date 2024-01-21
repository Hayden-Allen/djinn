#include "pch.h"
#include "camera_entity.h"

namespace djinn
{
	camera_entity::camera_entity(id_t const id, std::string const& fp, JSRuntime* const runtime) :
		entity(id, fp, runtime),
		m_proj(pmat_util::projection(108 * 9.f / 16.f, 16.f / 9.f, .1, 1000)),
		m_view(m_transform.invert_cast_copy<space::WORLD, space::CAMERA>()),
		m_view_proj(m_proj * m_view),
		m_view_proj_basis(m_proj * m_view.basis_copy())
	{}



	void camera_entity::update_mats()
	{
		m_view = get_world_transform().invert_cast_copy<space::WORLD, space::CAMERA>();
		m_view_proj = m_proj * m_view;
		m_view_proj_basis = m_proj * m_view.basis_copy();
	}
	void camera_entity::configure(f32 const fov_y, f32 const aspect, f32 const near, f32 const far)
	{
		m_proj = pmat_util::projection(fov_y, aspect, near, far);
		update_mats();
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
	mat<space::WORLD, space::CLIP> const& camera_entity::get_view_proj_basis() const
	{
		return m_view_proj_basis;
	}
} // namespace djinn

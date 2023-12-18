#pragma once
#include "pch.h"
#include "entity.h"

namespace djinn
{
	class camera_entity final : public entity
	{
	public:
		camera_entity(std::string const& fp, JSRuntime* const runtime);
		DCM(camera_entity);
	public:
		void configure(f32 const fov_y, f32 const aspect, f32 const near, f32 const far);
		pmat<space::CAMERA, space::CLIP> const& get_proj() const;
		tmat<space::WORLD, space::CAMERA> const& get_view() const;
		mat<space::WORLD, space::CLIP> const& get_view_proj() const;
	private:
		pmat<space::CAMERA, space::CLIP> m_proj;
		tmat<space::WORLD, space::CAMERA> m_view;
		mat<space::WORLD, space::CLIP> m_vp;
	};
} // namespace djinn

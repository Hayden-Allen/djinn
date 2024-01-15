#pragma once
#include "pch.h"
#include "entity.h"

namespace djinn
{
	template<typename T>
	class entity_manager_base;

	class camera_entity final : public entity
	{
		friend class entity_manager_base<camera_entity>;
	public:
		camera_entity(id_t const id, std::string const& fp, JSRuntime* const runtime);
		DCM(camera_entity);
	public:
		void update(f32 const dt, f32 const time) override;
		void update_mats();
		void configure(f32 const fov_y, f32 const aspect, f32 const near, f32 const far);
		pmat<space::CAMERA, space::CLIP> const& get_proj() const;
		tmat<space::WORLD, space::CAMERA> const& get_view() const;
		mat<space::WORLD, space::CLIP> const& get_view_proj() const;
	private:
		pmat<space::CAMERA, space::CLIP> m_proj;
		tmat<space::WORLD, space::CAMERA> m_view;
		mat<space::WORLD, space::CLIP> m_view_proj;
		mat<space::WORLD, space::CLIP> m_view_proj_basis;
	};
} // namespace djinn

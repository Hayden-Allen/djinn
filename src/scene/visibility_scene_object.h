#pragma once
#include "pch.h"
#include "scene_object.h"

namespace djinn
{
	class visibility_scene_object : public scene_object
	{
	public:
		DCM(visibility_scene_object);
		virtual ~visibility_scene_object();
	public:
		tmat<space::OBJECT, space::WORLD> get_graphics_transform() const override;
		void set_visible(bool const visible);
	protected:
		bool m_visible;
	protected:
		visibility_scene_object(id_t const id);
	};
} // namespace djinn

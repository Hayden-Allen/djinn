#pragma once
#include "pch.h"
#include "scene/visibility_scene_object.h"
#include "xport.h"

namespace djinn
{
	class light final : public visibility_scene_object, public xport
	{
		enum class light_type
		{
			NONE = -1,
			DIRECTION,
			POINT,
			AREA,
			SPOT,
			COUNT
		};
	public:
		light(id_t const id);
		light(id_t const id, haul::input_file* const in);
		DCM(light);
	public:
		tmat<space::OBJECT, space::WORLD> get_graphics_transform() const override;
		void update_transform();
		mgl::light const& get_raw() const;
		mgl::light& get_raw();
	private:
		mgl::light m_raw;
		light_type m_type;
	};
} // namespace djinn

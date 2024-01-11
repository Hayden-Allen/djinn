#pragma once
#include "pch.h"
#include "visibility_scene_object.h"

namespace djinn
{
	class light final : public visibility_scene_object
	{
	public:
		light(id_t const id);
		light(id_t const id, mgl::input_file* const in);
		DCM(light);
	public:
		void update_transform() override;
		mgl::light const& get_raw() const;
		mgl::light& get_raw();
	private:
		mgl::light m_raw;
	};
} // namespace djinn

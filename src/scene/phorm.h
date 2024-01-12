#pragma once
#include "pch.h"
#include "visibility_scene_object.h"
#include "xport.h"

namespace djinn
{
	class shaders;
	class material;

	class phorm final : public visibility_scene_object, public xport
	{
	public:
		phorm(id_t const id, mgl::input_file* const in, std::unordered_map<u32, sptr<material>> const& mats);
		DCM(phorm);
	public:
		void draw(sptr<mgl::context> const& ctx);
		void set_shaders(sptr<shaders> const& shaders);
		std::unordered_map<sptr<material>, static_retained_render_object> const& get_render_objects() const;
	private:
		std::unordered_map<sptr<material>, static_retained_render_object> m_ros;
		wptr<shaders> m_shaders;
	};
} // namespace djinn

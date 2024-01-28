#pragma once
#include "pch.h"
#include "scene/visibility_scene_object.h"
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
		void set_alpha_shaders(sptr<shaders> const& alpha_shaders);
		std::unordered_map<material const*, static_retained_render_object> const& get_render_objects() const;
		u64 const get_vertex_count() const;
	private:
		std::unordered_map<material const*, static_retained_render_object> m_ros;
		wptr<shaders> m_shaders;
		wptr<shaders> m_alpha_shaders;
	private:
		void shader_setup(wptr<shaders> shaders);
	};
} // namespace djinn

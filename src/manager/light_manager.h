#pragma once
#include "manager.h"
#include "scene/light.h"

namespace djinn
{
	class light_manager final : public ref_counted_manager<light>
	{
	public:
		light_manager();
		DCM(light_manager);
	public:
		id_t create();
		void destroy(id_t const id) override;
		void update();
	private:
		static constexpr u64 s_light_offset = 16, s_floats_per_light = sizeof(mgl::light) / sizeof(f32); // sizeof(vec4)
	private:
		std::unordered_map<id_t, u64> m_id2idx;
		std::vector<sptr<light>> m_lights;
		dynamic_uniform_buffer m_ubo;
		std::vector<mgl::light> m_raw;
	};
} // namespace djinn

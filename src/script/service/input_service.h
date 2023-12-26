#pragma once
#include "pch.h"
#include "service.h"

namespace djinn
{
	struct mapped_gamepad
	{
		f32 left_x = 0.0f, left_y = 0.0f, left_deadzone = 0.1f;
		f32 right_x = 0.0f, right_y = 0.0f, right_deadzone = 0.1f;
		f32 lt = 0.0f, rt = 0.0f, trigger_deadzone = 0.1f;
		bool a = false, b = false, x = false, y = false;
		bool up = false, down = false, left = false, right = false;
		bool lb = false, rb = false;
	};

	struct key_to_gamepad_mapping
	{
		int left_x_neg = GLFW_KEY_A, left_x_pos = GLFW_KEY_D, left_y_neg = GLFW_KEY_W, left_y_pos = GLFW_KEY_S;
		int right_x_neg = GLFW_KEY_LEFT, right_x_pos = GLFW_KEY_RIGHT, right_y_neg = GLFW_KEY_UP, right_y_pos = GLFW_KEY_DOWN;
		int lt = GLFW_KEY_LEFT_BRACKET, rt = GLFW_KEY_RIGHT_BRACKET;
		int a = GLFW_KEY_X, b = GLFW_KEY_C, x = GLFW_KEY_V, y = GLFW_KEY_B;
		int up = GLFW_KEY_I, down = GLFW_KEY_K, left = GLFW_KEY_J, right = GLFW_KEY_L;
		int lb = GLFW_KEY_SEMICOLON, rb = GLFW_KEY_APOSTROPHE;
	};

	class input_service final : public haul::parent<service<input_service>>
	{
		friend class super;
	public:
		DCM(input_service);
	public:
		static void init(sptr<mgl::context> const& context);
		static void register_functions(JSContext* const ctx);
		static sptr<mgl::context> get_context();
		static mapped_gamepad const& get_gamepad();
		static void update();
	private:
		static f32 apply_deadzone(f32 const x, f32 const deadzone);
		static f32 key2f32(int const i);
		static bool key2bool(int const i);
	private:
		sptr<mgl::context> m_context;
		mapped_gamepad m_gamepad;
		key_to_gamepad_mapping m_mapping;
	private:
		input_service(sptr<mgl::context> const& context);
	};
} // namespace djinn

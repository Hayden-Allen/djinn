#include "pch.h"
#include "input_service.h"
#include "script/js.h"

namespace djinn::js::input_service
{
	JSValue get_key(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		s32 const key = js::extract_s32(ctx, argv[0]);
		return js::create_u32(ctx, ::djinn::input_service::get_context()->get_key(key));
	}
	JSValue get_key_diff(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		s32 const key1 = js::extract_s32(ctx, argv[0]);
		s32 const key2 = js::extract_s32(ctx, argv[1]);
		return js::create_f32(ctx, (f32)::djinn::input_service::get_context()->get_key(key1) - (f32)::djinn::input_service::get_context()->get_key(key2));
	}
	JSValue left_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_f32(ctx, ::djinn::input_service::get_gamepad().left_x);
	}
	JSValue left_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_f32(ctx, ::djinn::input_service::get_gamepad().left_y);
	}
	JSValue right_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_f32(ctx, ::djinn::input_service::get_gamepad().right_x);
	}
	JSValue right_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_f32(ctx, ::djinn::input_service::get_gamepad().right_y);
	}
	JSValue button_a(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_bool(ctx, ::djinn::input_service::get_gamepad().a);
	}
	JSValue button_b(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_bool(ctx, ::djinn::input_service::get_gamepad().b);
	}
	JSValue button_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_bool(ctx, ::djinn::input_service::get_gamepad().x);
	}
	JSValue button_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_bool(ctx, ::djinn::input_service::get_gamepad().y);
	}
} // namespace djinn::js::input_service



namespace djinn
{
	void input_service::init(sptr<mgl::context> const& context)
	{
		ASSERT(!s_instance);
		s_instance = new input_service(context);
	}
	void input_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "getKey", 1, js::input_service::get_key);
		super::register_function(ctx, "getKeyDiff", 2, js::input_service::get_key_diff);
		super::register_function(ctx, "leftX", 0, js::input_service::left_x);
		super::register_function(ctx, "leftY", 0, js::input_service::left_y);
		super::register_function(ctx, "rightX", 0, js::input_service::right_x);
		super::register_function(ctx, "rightY", 0, js::input_service::right_y);
		super::register_function(ctx, "buttonA", 0, js::input_service::button_a);
		super::register_function(ctx, "buttonB", 0, js::input_service::button_b);
		super::register_function(ctx, "buttonX", 0, js::input_service::button_x);
		super::register_function(ctx, "buttonY", 0, js::input_service::button_y);

		char buf[32] = { 0 };
		for (char i = '0'; i <= '9'; i++)
		{
			sprintf_s(buf, 32, "KEY_%c", i);
			super::register_property_u32(ctx, buf, (u32)i);
		}
		for (char i = 'A'; i <= 'Z'; i++)
		{
			sprintf_s(buf, 32, "KEY_%c", i);
			super::register_property_u32(ctx, buf, (u32)i);
		}
		super::register_property_u32(ctx, "KEY_RIGHT", GLFW_KEY_RIGHT);
		super::register_property_u32(ctx, "KEY_LEFT", GLFW_KEY_LEFT);
		super::register_property_u32(ctx, "KEY_DOWN", GLFW_KEY_DOWN);
		super::register_property_u32(ctx, "KEY_UP", GLFW_KEY_UP);
		super::register_property_u32(ctx, "KEY_SPACE", GLFW_KEY_SPACE);
		super::register_property_u32(ctx, "KEY_SHIFT", GLFW_KEY_LEFT_SHIFT);
		super::register_property_u32(ctx, "KEY_LEFT_CONTROL", GLFW_KEY_LEFT_CONTROL);
		super::register_property_u32(ctx, "KEY_ENTER", GLFW_KEY_ENTER);
		super::register_property_u32(ctx, "KEY_BACKSPACE", GLFW_KEY_BACKSPACE);
	}
	sptr<mgl::context> input_service::get_context()
	{
		return s_instance->m_context;
	}
	mapped_gamepad const& input_service::get_gamepad()
	{
		return s_instance->m_gamepad;
	}
	void input_service::update()
	{
		int first_connected_gamepad = -1;
		for (int i = 0; i < 16; i++)
		{
			if (get_context()->gamepad_available[i])
			{
				first_connected_gamepad = i;
				break;
			}
		}

		mapped_gamepad& m_gamepad = s_instance->m_gamepad;
		key_to_gamepad_mapping const& m_mapping = s_instance->m_mapping;
		if (first_connected_gamepad >= 0)
		{
			GLFWgamepadstate const& gamepad = get_context()->gamepad[first_connected_gamepad];
			m_gamepad.left_x = apply_deadzone(gamepad.axes[GLFW_GAMEPAD_AXIS_LEFT_X], m_gamepad.left_deadzone);
			m_gamepad.left_y = apply_deadzone(gamepad.axes[GLFW_GAMEPAD_AXIS_LEFT_Y], m_gamepad.left_deadzone);
			m_gamepad.right_x = apply_deadzone(gamepad.axes[GLFW_GAMEPAD_AXIS_RIGHT_X], m_gamepad.right_deadzone);
			m_gamepad.right_y = apply_deadzone(gamepad.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y], m_gamepad.right_deadzone);
			m_gamepad.lt = apply_deadzone(gamepad.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER], m_gamepad.trigger_deadzone);
			m_gamepad.rt = apply_deadzone(gamepad.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER], m_gamepad.trigger_deadzone);
			m_gamepad.a = gamepad.buttons[GLFW_GAMEPAD_BUTTON_A];
			m_gamepad.b = gamepad.buttons[GLFW_GAMEPAD_BUTTON_B];
			m_gamepad.x = gamepad.buttons[GLFW_GAMEPAD_BUTTON_X];
			m_gamepad.y = gamepad.buttons[GLFW_GAMEPAD_BUTTON_Y];
			m_gamepad.up = gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP];
			m_gamepad.down = gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN];
			m_gamepad.left = gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT];
			m_gamepad.right = gamepad.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT];
			m_gamepad.lb = gamepad.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER];
			m_gamepad.rb = gamepad.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER];
		}
		else
		{
			m_gamepad.left_x = key2f32(m_mapping.left_x_pos) - key2f32(m_mapping.left_x_neg);
			m_gamepad.left_y = key2f32(m_mapping.left_y_pos) - key2f32(m_mapping.left_y_neg);
			m_gamepad.right_x = key2f32(m_mapping.right_x_pos) - key2f32(m_mapping.right_x_neg);
			m_gamepad.right_y = key2f32(m_mapping.right_y_pos) - key2f32(m_mapping.right_y_neg);
			m_gamepad.lt = key2f32(m_mapping.lt);
			m_gamepad.rt = key2f32(m_mapping.rt);
			m_gamepad.a = key2bool(m_mapping.a);
			m_gamepad.b = key2bool(m_mapping.b);
			m_gamepad.x = key2bool(m_mapping.x);
			m_gamepad.y = key2bool(m_mapping.y);
			m_gamepad.up = key2bool(m_mapping.up);
			m_gamepad.down = key2bool(m_mapping.down);
			m_gamepad.left = key2bool(m_mapping.left);
			m_gamepad.right = key2bool(m_mapping.right);
			m_gamepad.lb = key2bool(m_mapping.lb);
			m_gamepad.rb = key2bool(m_mapping.rb);
		}
	}



	f32 input_service::apply_deadzone(f32 const x, f32 const deadzone)
	{
		f32 const sign = x < 0.0f ? -1.0f : 1.0f;
		return sign * std::max(0.0f, std ::fabs(x) - deadzone) / (1.0f - deadzone);
	}
	f32 input_service::key2f32(int const i)
	{
		return get_context()->get_key(i);
	}
	bool input_service::key2bool(int const i)
	{
		return get_context()->get_key(i);
	}



	input_service::input_service(sptr<mgl::context> const& context) :
		haul::parent<service<input_service>>("Input"),
		m_context(context)
	{}
} // namespace djinn

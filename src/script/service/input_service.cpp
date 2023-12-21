#include "pch.h"
#include "input_service.h"
#include "script/js.h"

namespace djinn::js::input_service
{
	JSValue get_key(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		s32 const key = js::extract_s32(ctx, argv[0]);
		return js::create_bool(ctx, ::djinn::input_service::get_context()->get_key(key));
	}
	JSValue left_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_f32(ctx, 1.f * ::djinn::input_service::get_context()->get_key(GLFW_KEY_D) - ::djinn::input_service::get_context()->get_key(GLFW_KEY_A));
	}
	JSValue left_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_f32(ctx, 1.f * ::djinn::input_service::get_context()->get_key(GLFW_KEY_S) - ::djinn::input_service::get_context()->get_key(GLFW_KEY_W));
	}
	JSValue right_x(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_f32(ctx, 1.f * ::djinn::input_service::get_context()->get_key(GLFW_KEY_RIGHT) - ::djinn::input_service::get_context()->get_key(GLFW_KEY_LEFT));
	}
	JSValue right_y(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 0);
		return js::create_f32(ctx, 1.f * ::djinn::input_service::get_context()->get_key(GLFW_KEY_DOWN) - ::djinn::input_service::get_context()->get_key(GLFW_KEY_UP));
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
		super::register_function(ctx, "leftX", 0, js::input_service::left_x);
		super::register_function(ctx, "leftY", 0, js::input_service::left_y);
		super::register_function(ctx, "rightX", 0, js::input_service::right_x);
		super::register_function(ctx, "rightY", 0, js::input_service::right_y);

		char buf[32] = { 0 };
		for (char i = '0'; i < '9'; i++)
		{
			sprintf_s(buf, "KEY_%c", i);
			super::register_property_u32(ctx, buf, (u32)i);
		}
		for (char i = 'A'; i < 'Z'; i++)
		{
			sprintf_s(buf, "KEY_%c", i);
			super::register_property_u32(ctx, buf, (u32)i);
		}
		super::register_property_u32(ctx, "KEY_RIGHT", GLFW_KEY_RIGHT);
		super::register_property_u32(ctx, "KEY_LEFT", GLFW_KEY_LEFT);
		super::register_property_u32(ctx, "KEY_DOWN", GLFW_KEY_DOWN);
		super::register_property_u32(ctx, "KEY_UP", GLFW_KEY_UP);
		super::register_property_u32(ctx, "KEY_SPACE", GLFW_KEY_SPACE);
		super::register_property_u32(ctx, "KEY_SHIFT", GLFW_KEY_LEFT_SHIFT);
	}
	sptr<mgl::context> input_service::get_context()
	{
		return s_instance->m_context;
	}



	input_service::input_service(sptr<mgl::context> const& context) :
		haul::parent<service<input_service>>("Input"),
		m_context(context)
	{}
} // namespace djinn

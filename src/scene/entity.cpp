#include "pch.h"
#include "entity.h"
#include "script/service/asset_service.h"
#include "script/service/render_service.h"
#include "script/service/nanovg_service.h"
#include "script/service/util_service.h"
#include "script/service/scene_service.h"
#include "script/service/input_service.h"
#include "script/js.h"

namespace djinn
{
	entity::entity(id_t const id, std::string const& fp, JSRuntime* const runtime) :
		m_ctx(JS_NewContext(runtime)),
		m_id(id)
	{
		js::global::init_globals(m_ctx);
		asset_service::register_functions(m_ctx);
		render_service::register_functions(m_ctx);
		nanovg_service::register_functions(m_ctx);
		util_service::register_functions(m_ctx);
		scene_service::register_functions(m_ctx);
		input_service::register_functions(m_ctx);
	}
	entity::~entity()
	{
		if (m_script_loaded)
		{
			call_unload();
			call_destroy();
			JS_FreeValue(m_ctx, m_this);
		}
		JS_FreeContext(m_ctx);
	}



	void entity::update(f32 const dt)
	{
		call_main(dt);
	}
	void entity::draw()
	{
		call_reserved("__draw", 0, nullptr);
	}
	void entity::draw_ui()
	{
		call_reserved("__ui", 0, nullptr);
	}
	tmat<space::OBJECT, space::WORLD> const& entity::get_transform() const
	{
		return m_transform;
	}



	void entity::inject_script(std::string const& fp, std::string const& src)
	{
		if (m_script_loaded)
			call_unload();

		JSValue const eval_ret = JS_Eval(m_ctx, src.data(), src.size(), fp.c_str(), JS_EVAL_TYPE_MODULE);
		check_exception(eval_ret, "entity::inject_script: " + fp);
		JS_FreeValue(m_ctx, eval_ret);

		if (!m_script_loaded)
		{
			JSValue const global = JS_GetGlobalObject(m_ctx);
			JSValue const exports_def = JS_GetPropertyStr(m_ctx, global, "__djinnExports");
			JSValue const default_def = JS_GetPropertyStr(m_ctx, exports_def, "default");
			m_this = JS_CallConstructor(m_ctx, default_def, 0, nullptr);
			JS_SetPropertyStr(m_ctx, m_this, "id", JS_NewUint32(m_ctx, m_id));
			JS_FreeValue(m_ctx, default_def);
			JS_FreeValue(m_ctx, exports_def);
			JS_FreeValue(m_ctx, global);
			call_init();
		}

		call_load();
		m_script_loaded = true;
	}
	void entity::call_reserved(std::string const& name, s32 const argc, JSValue* const argv)
	{
		JSValue const global = JS_GetGlobalObject(m_ctx);
		JSValue const exports_def = JS_GetPropertyStr(m_ctx, global, "__djinnExports");
		JSValue const default_def = JS_GetPropertyStr(m_ctx, exports_def, "default");
		JSValue const default_proto = JS_GetPropertyStr(m_ctx, default_def, "prototype");
		JSValue const fn = JS_GetPropertyStr(m_ctx, default_proto, name.c_str());

		JSValue const call_ret = JS_Call(m_ctx, fn, m_this, argc, argv);
		check_exception(call_ret, "entity::call_reserved: " + name);
		JS_FreeValue(m_ctx, call_ret);

		JS_FreeValue(m_ctx, fn);
		JS_FreeValue(m_ctx, default_proto);
		JS_FreeValue(m_ctx, default_def);
		JS_FreeValue(m_ctx, exports_def);
		JS_FreeValue(m_ctx, global);
	}
	void entity::call_load()
	{
		call_reserved("__load", 0, nullptr);
	}
	void entity::call_unload()
	{
		call_reserved("__unload", 0, nullptr);
	}
	void entity::call_init()
	{
		call_reserved("__init", 0, nullptr);
	}
	void entity::call_destroy()
	{
		call_reserved("__destroy", 0, nullptr);
	}
	void entity::call_main(f32 const dt)
	{
		JSValue dtval = JS_NewFloat64(m_ctx, dt);
		call_reserved("__main", 1, &dtval);
		JS_FreeValue(m_ctx, dtval);
	}
	void entity::check_exception(JSValue const val, std::string const& msg)
	{
		if (JS_IsException(val))
		{
			JSValue const ex_val = JS_GetException(m_ctx);
			char const* const ex = JS_ToCString(m_ctx, ex_val);
			printf("Exception occurred: %s: %s\n", ex, msg.c_str());
			JS_FreeCString(m_ctx, ex);
			JS_FreeValue(m_ctx, ex_val);
		}
	}
} // namespace djinn

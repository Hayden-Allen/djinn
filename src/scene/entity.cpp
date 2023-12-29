#include "pch.h"
#include "entity.h"
#include "script/service/asset_service.h"
#include "script/service/render_service.h"
#include "script/service/nanovg_service.h"
#include "script/service/util_service.h"
#include "script/service/scene_service.h"
#include "script/service/input_service.h"
#include "script/service/imgui_service.h"
#include "script/service/sound_service.h"
#include "script/js.h"

namespace djinn
{
	entity::entity(id_t const id, std::string const& fp, JSRuntime* const runtime) :
		scene_object(id),
		m_ctx(JS_NewContext(runtime))
	{
		js::global::init_globals(m_ctx);
		asset_service::register_functions(m_ctx);
		render_service::register_functions(m_ctx);
		nanovg_service::register_functions(m_ctx);
		util_service::register_functions(m_ctx);
		scene_service::register_functions(m_ctx);
		input_service::register_functions(m_ctx);
		imgui_service::register_functions(m_ctx);
		sound_service::register_functions(m_ctx);
	}
	entity::~entity()
	{
		if (m_script_loaded)
		{
			// call_unload();
			call_destroy();
			JS_FreeValue(m_ctx, m_this);
			clear_cache();
		}
		JS_FreeContext(m_ctx);
	}



	void entity::update(f32 const dt)
	{
		m_request_imgui = false;
		call_main(dt);
		update_transform();
	}
	void entity::draw()
	{
		call_reserved("__draw", 0, nullptr);
	}
	void entity::draw_ui()
	{
		call_reserved("__ui", 0, nullptr);
	}
	void entity::draw_imgui()
	{
		if (!m_request_imgui)
			return;
		char buf[128] = { 0 };
		sprintf_s(buf, "Entity_%u", m_id);
		if (ImGui::Begin(buf))
		{
			call_reserved("__imgui", 0, nullptr);
		}
		ImGui::End();
	}
	void entity::request_imgui()
	{
		m_request_imgui = true;
	}
	JSValue entity::get_js_value()
	{
		return m_this;
	}



	void entity::inject_script(std::string const& fp, std::string const& src)
	{
		if (m_script_loaded)
		{
			call_unload();
			clear_cache();
		}

		JSValue const eval_ret = JS_Eval(m_ctx, src.data(), src.size(), fp.c_str(), JS_EVAL_TYPE_MODULE);
		check_exception(eval_ret, "entity::inject_script: " + fp);
		JS_FreeValue(m_ctx, eval_ret);

		m_global = JS_GetGlobalObject(m_ctx);
		m_exports = JS_GetPropertyStr(m_ctx, m_global, "__djinnExports");
		m_default = JS_GetPropertyStr(m_ctx, m_exports, "default");
		m_proto = JS_GetPropertyStr(m_ctx, m_default, "prototype");
		if (!m_script_loaded)
		{
			m_this = JS_CallConstructor(m_ctx, m_default, 0, nullptr);
			JS_SetPropertyStr(m_ctx, m_this, "id", JS_NewUint32(m_ctx, m_id));
			call_init();
		}

		call_load();
		m_script_loaded = true;
	}
	void entity::call_reserved(std::string const& name, s32 const argc, JSValue* const argv)
	{
		auto const& it = m_function_cache.find(name);
		JSValue fn = JS_UNDEFINED;
		if (it != m_function_cache.end())
		{
			fn = it->second;
		}
		else
		{
			fn = JS_GetPropertyStr(m_ctx, m_proto, name.c_str());
			m_function_cache.insert({ name, fn });
		}

		JSValue const call_ret = JS_Call(m_ctx, fn, m_this, argc, argv);
#ifndef DJINN_DIST
		check_exception(call_ret, "entity::call_reserved: " + name);
#endif
		JS_FreeValue(m_ctx, call_ret);
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
	void entity::check_exception(JSValue const val, std::string const& msg) const
	{
#ifndef DJINN_DIST
		if (JS_IsException(val))
		{
			JSValue const ex_val = JS_GetException(m_ctx);
			char const* const ex = JS_ToCString(m_ctx, ex_val);
			printf("Exception occurred: %s: %s\n", ex, msg.c_str());
			JS_FreeCString(m_ctx, ex);
			JS_FreeValue(m_ctx, ex_val);
		}
#endif
	}
	void entity::clear_cache()
	{
		for (auto& pair : m_function_cache)
			JS_FreeValue(m_ctx, pair.second);
		m_function_cache.clear();
		JS_FreeValue(m_ctx, m_proto);
		JS_FreeValue(m_ctx, m_default);
		JS_FreeValue(m_ctx, m_exports);
		JS_FreeValue(m_ctx, m_global);
	}
} // namespace djinn

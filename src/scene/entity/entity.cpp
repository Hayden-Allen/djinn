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
#include "script/service/event_service.h"
#include "script/js.h"
#include "scene/xport/phorm.h"

namespace djinn
{
	entity::entity(id_t const id, std::string const& fp, JSRuntime* const runtime) :
		scene_object(id),
		m_ctx(JS_NewContext(runtime)),
		m_imgui_name("Entity_" + std::to_string(id))
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
		event_service::register_functions(m_ctx);
	}
	entity::~entity()
	{
		if (m_script_loaded)
		{
			call_unload();
			call_destroy();
			JS_FreeValue(m_ctx, m_this);
			clear_cache();
		}
		event_service::unsubscribe_all(m_ctx);
		JS_FreeContext(m_ctx);
	}



	bool entity::is_entity() const
	{
		return true;
	}
	void entity::update(f32 const dt, f32 const time)
	{
		m_request_imgui = false;
		call_main(dt, time);
	}
	void entity::request_imgui()
	{
		m_request_imgui = true;
	}
	void entity::set_imgui_name(std::string const& name)
	{
		m_imgui_name = name;
	}
	JSValue entity::get_js_value()
	{
		return m_this;
	}



	void entity::call_draw()
	{
		call_reserved("__draw", 0, nullptr);
	}
	void entity::call_ui()
	{
		call_reserved("__ui", 0, nullptr);
	}
	void entity::call_imgui()
	{
		if (!m_request_imgui)
			return;
		if (ImGui::Begin(m_imgui_name.c_str()))
		{
			call_reserved("__imgui", 0, nullptr);
		}
		ImGui::End();
	}
	void entity::call_collide(entity* const other, direction<space::WORLD> const& normal)
	{
		JSValue args[2] = {
			JS_DupValue(m_ctx, other->get_js_value()),
			js::create_f32_array(m_ctx, 3, normal.e)
		};
		call_reserved("__collide_entity", 2, args);
		for (u32 i = 0; i < 2; i++)
			JS_FreeValue(m_ctx, args[i]);
	}
	void entity::call_collide(phorm* const phorm, direction<space::WORLD> const& normal)
	{
		JSValue args[2] = {
			js::create_id(m_ctx, phorm->get_id()),
			js::create_f32_array(m_ctx, 3, normal.e)
		};
		call_reserved("__collide_phorm", 2, args);
		for (u32 i = 0; i < 2; i++)
			JS_FreeValue(m_ctx, args[i]);
	}
	void entity::call_no_collide()
	{
		call_reserved("__no_collide", 0, nullptr);
	}



	void entity::inject_script(std::string const& fp, std::string const& src, s32 const argc, JSValueConst* const argv)
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
			JS_SetPropertyStr(m_ctx, m_this, "id", js::create_id(m_ctx, m_id));
			call_init(argc, argv);
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
		check_exception(call_ret, "entity::" + name);
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
	void entity::call_init(s32 const argc, JSValueConst* const argv)
	{
		call_reserved("__init", argc, argv);
	}
	void entity::call_destroy()
	{
		call_reserved("__destroy", 0, nullptr);
	}
	void entity::call_main(f32 const dt, f32 const time)
	{
		JSValue dtval = JS_NewFloat64(m_ctx, dt);
		JSValue timeval = JS_NewFloat64(m_ctx, time);
		JSValue args[2] = { dtval, timeval };
		call_reserved("__main", 2, args);
		JS_FreeValue(m_ctx, timeval);
		JS_FreeValue(m_ctx, dtval);
	}
	void entity::check_exception(JSValue const val, std::string const& msg) const
	{
#ifndef DJINN_DIST
		if (JS_IsException(val))
		{
			JSValue const ex_val = JS_GetException(m_ctx);
			char const* const ex = JS_ToCString(m_ctx, ex_val);
			printf("%s in (%s)\n", ex, msg.c_str());
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

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
		tmat<space::OBJECT, space::PARENT> const& trans = tmat_util::translation<space::OBJECT, space::PARENT>(point<space::PARENT>(m_pos[0], m_pos[1], m_pos[2]));
		tmat<space::OBJECT, space::OBJECT> const& rot = tmat_util::rotation_yxz<space::OBJECT>(m_rot[0], m_rot[1], m_rot[2]);
		tmat<space::OBJECT, space::OBJECT> const& scale = tmat_util::scale<space::OBJECT>(m_scale[0], m_scale[1], m_scale[2]);
		m_transform = trans * rot * scale;
	}
	void entity::draw()
	{
		call_reserved("__draw", 0, nullptr);
	}
	void entity::draw_ui()
	{
		call_reserved("__ui", 0, nullptr);
	}
	JSValue entity::get_js_value()
	{
		return m_this;
	}
	void entity::set_pos(f32 const x, f32 const y, f32 const z)
	{
		m_pos[0] = x;
		m_pos[1] = y;
		m_pos[2] = z;
	}
	void entity::set_rot(f32 const x, f32 const y, f32 const z)
	{
		m_rot[0] = x;
		m_rot[1] = y;
		m_rot[2] = z;
	}
	void entity::set_scale(f32 const x, f32 const y, f32 const z)
	{
		ASSERT(x != 0 && y != 0 && z != 0);
		m_scale[0] = x;
		m_scale[1] = y;
		m_scale[2] = z;
	}
	f32 const* entity::get_pos() const
	{
		return m_pos;
	}
	f32 const* entity::get_rot() const
	{
		return m_rot;
	}
	f32 const* entity::get_scale() const
	{
		return m_scale;
	}
	tmat<space::OBJECT, space::PARENT> const& entity::get_transform() const
	{
		return m_transform;
	}
	tmat<space::OBJECT, space::WORLD> entity::get_world_transform() const
	{
		ASSERT(false);
		return m_transform.cast_copy<space::OBJECT, space::WORLD>();
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

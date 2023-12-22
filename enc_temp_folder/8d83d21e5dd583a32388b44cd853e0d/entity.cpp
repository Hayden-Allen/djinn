#include "pch.h"
#include "entity.h"
#include "script/service/asset_service.h"
#include "script/service/render_service.h"
#include "script/service/nanovg_service.h"
#include "script/service/util_service.h"
#include "script/service/scene_service.h"
#include "script/service/input_service.h"
#include "script/service/imgui_service.h"
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
		imgui_service::register_functions(m_ctx);
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
		JS_FreeContext(m_ctx);
	}



	void entity::update(f32 const dt)
	{
		m_request_imgui = false;
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



	f32 const* entity::get_pos() const
	{
		return m_pos;
	}
	void entity::set_pos(f32 const x, f32 const y, f32 const z)
	{
		m_pos[0] = x;
		m_pos[1] = y;
		m_pos[2] = z;
	}
	void entity::set_pos_x(f32 const x)
	{
		m_pos[0] = x;
	}
	void entity::set_pos_y(f32 const y)
	{
		m_pos[1] = y;
	}
	void entity::set_pos_z(f32 const z)
	{
		m_pos[2] = z;
	}
	void entity::add_pos(f32 const x, f32 const y, f32 const z)
	{
		m_pos[0] += x;
		m_pos[1] += y;
		m_pos[2] += z;
	}
	void entity::add_pos_local(f32 const x, f32 const y, f32 const z)
	{
		vec<space::OBJECT> const local(x, y, z);
		vec<space::PARENT> const parent = m_transform * local;
		m_pos[0] += parent.x;
		m_pos[1] += parent.y;
		m_pos[2] += parent.z;
	}
	f32 const* entity::get_rot() const
	{
		return m_rot;
	}
	void entity::set_rot(f32 const x, f32 const y, f32 const z)
	{
		m_rot[0] = x;
		m_rot[1] = y;
		m_rot[2] = z;
	}
	void entity::set_rot_x(f32 const x)
	{
		m_rot[0] = x;
	}
	void entity::set_rot_y(f32 const y)
	{
		m_rot[1] = y;
	}
	void entity::set_rot_z(f32 const z)
	{
		m_rot[2] = z;
	}
	void entity::add_rot(f32 const x, f32 const y, f32 const z)
	{
		m_rot[0] += x;
		m_rot[1] += y;
		m_rot[2] += z;
	}
	void entity::add_rot_local(f32 const x, f32 const y, f32 const z)
	{
		vec<space::OBJECT> const local(x, y, z);
		vec<space::PARENT> const parent = m_transform * local;
		m_rot[0] += parent.x;
		m_rot[1] += parent.y;
		m_rot[2] += parent.z;
	}
	f32 const* entity::get_scale() const
	{
		return m_scale;
	}
	void entity::set_scale(f32 const x, f32 const y, f32 const z)
	{
		ASSERT(x != 0 && y != 0 && z != 0)
		m_scale[0] = x;
		m_scale[1] = y;
		m_scale[2] = z;
	}
	void entity::set_scale_x(f32 const x)
	{
		ASSERT(x != 0)
		m_scale[0] = x;
	}
	void entity::set_scale_y(f32 const y)
	{
		ASSERT(y != 0)
		m_scale[1] = y;
	}
	void entity::set_scale_z(f32 const z)
	{
		ASSERT(z != 0)
		m_scale[2] = z;
	}
	void entity::add_scale(f32 const x, f32 const y, f32 const z)
	{
		m_scale[0] += x;
		m_scale[1] += y;
		m_scale[2] += z;
		ASSERT(m_scale[0] != 0 && m_scale[1] != 0 && m_scale[2] != 0)
	}
	void entity::add_scale_local(f32 const x, f32 const y, f32 const z)
	{
		vec<space::OBJECT> const local(x, y, z);
		vec<space::PARENT> const parent = m_transform * local;
		m_rot[0] += parent.x;
		m_rot[1] += parent.y;
		m_rot[2] += parent.z;
		ASSERT(m_scale[0] != 0 && m_scale[1] != 0 && m_scale[2] != 0)
	}
	tmat<space::OBJECT, space::PARENT> const& entity::get_transform() const
	{
		return m_transform;
	}
	tmat<space::OBJECT, space::WORLD> entity::get_world_transform() const
	{
		ASSERT(false)
		return m_transform.cast_copy<space::OBJECT, space::WORLD>();
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
		if (JS_IsException(val))
		{
			JSValue const ex_val = JS_GetException(m_ctx);
			char const* const ex = JS_ToCString(m_ctx, ex_val);
			printf("Exception occurred: %s: %s\n", ex, msg.c_str());
			JS_FreeCString(m_ctx, ex);
			JS_FreeValue(m_ctx, ex_val);
		}
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

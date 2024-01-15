#pragma once
#include "pch.h"
#include "scene/scene_object_base.h"

namespace djinn
{
	template<typename T>
	class entity_manager_base;

	class entity : public scene_object_base
	{
		friend class entity_manager_base<entity>;
	public:
		entity(id_t const id, std::string const& fp, JSRuntime* const runtime);
		DCM(entity);
		virtual ~entity();
	public:
		virtual void update(f32 const dt, f32 const time);
		void draw();
		void draw_ui();
		void draw_imgui();
		void request_imgui();
		JSValue get_js_value();
	protected:
		JSContext* m_ctx;
		JSValue m_this = JS_UNDEFINED, m_global = JS_UNDEFINED, m_exports = JS_UNDEFINED, m_default = JS_UNDEFINED, m_proto = JS_UNDEFINED;
		std::unordered_map<std::string, JSValue> m_function_cache;
		bool m_script_loaded = false, m_request_imgui = false;
	protected:
		void inject_script(std::string const& fp, std::string const& src);
		void call_reserved(std::string const& name, s32 const argc, JSValue* const argv);
		void call_load();
		void call_unload();
		void call_init();
		void call_destroy();
		void call_main(f32 const dt, f32 const time);
		void check_exception(JSValue const val, std::string const& msg) const;
		void clear_cache();
	};
} // namespace djinn

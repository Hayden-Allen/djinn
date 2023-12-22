#pragma once
#include "pch.h"

namespace djinn
{
	template<typename T>
	class entity_manager_base;

	class entity
	{
		friend class entity_manager_base<entity>;
	public:
		entity(id_t const id, std::string const& fp, JSRuntime* const runtime);
		DCM(entity);
		virtual ~entity();
	public:
		virtual void update(f32 const dt);
		void draw();
		void draw_ui();
		void draw_imgui();
		void request_imgui();
		JSValue get_js_value();
		tmat<space::OBJECT, space::PARENT> const& get_transform() const;
		tmat<space::OBJECT, space::WORLD> get_world_transform() const;
	public:
		f32 const* get_pos() const;
		void set_pos(f32 const x, f32 const y, f32 const z);
		void set_pos_x(f32 const x);
		void set_pos_y(f32 const y);
		void set_pos_z(f32 const z);
		void add_pos(f32 const x, f32 const y, f32 const z);
		void add_pos_local(f32 const x, f32 const y, f32 const z);
	public:
		f32 const* get_rot() const;
		void set_rot(f32 const x, f32 const y, f32 const z);
		void set_rot_x(f32 const x);
		void set_rot_y(f32 const y);
		void set_rot_z(f32 const z);
		void add_rot(f32 const x, f32 const y, f32 const z);
		void add_rot_local(f32 const x, f32 const y, f32 const z);
	public:
		f32 const* get_scale() const;
		void set_scale(f32 const x, f32 const y, f32 const z);
		void set_scale_x(f32 const x);
		void set_scale_y(f32 const y);
		void set_scale_z(f32 const z);
		void add_scale(f32 const x, f32 const y, f32 const z);
		void add_scale_local(f32 const x, f32 const y, f32 const z);
	protected:
		JSContext* m_ctx;
		JSValue m_this = JS_UNDEFINED, m_global = JS_UNDEFINED, m_exports = JS_UNDEFINED, m_default = JS_UNDEFINED, m_proto = JS_UNDEFINED;
		std::unordered_map<std::string, JSValue> m_function_cache;
		tmat<space::OBJECT, space::PARENT> m_transform;
		f32 m_pos[3] = { 0 }, m_rot[3] = { 0 }, m_scale[3] = { 1, 1, 1 };
		id_t m_id;
		bool m_script_loaded = false, m_request_imgui = false;
	protected:
		void inject_script(std::string const& fp, std::string const& src);
		void call_reserved(std::string const& name, s32 const argc, JSValue* const argv);
		void call_load();
		void call_unload();
		void call_init();
		void call_destroy();
		void call_main(f32 const dt);
		void check_exception(JSValue const val, std::string const& msg) const;
		void clear_cache();
	};
} // namespace djinn

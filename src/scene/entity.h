#pragma once
#include "pch.h"


namespace djinn
{
	class entity_manager;

	class entity
	{
		friend class entity_manager;
	public:
		entity(id_t const id, std::string const& fp, JSRuntime* const runtime);
		DCM(entity);
		virtual ~entity();
	public:
		void update(f32 const dt);
		void draw();
		void draw_ui();
		tmat<space::OBJECT, space::WORLD> const& get_transform() const;
	protected:
		void inject_script(std::string const& fp, std::string const& src);
		void call_reserved(std::string const& name, s32 const argc, JSValue* const argv);
		void call_load();
		void call_unload();
		void call_init();
		void call_destroy();
		void call_main(f32 const dt);
		void check_exception(JSValue const val, std::string const& msg);
	protected:
		JSContext* m_ctx;
		JSValue m_this = JS_UNDEFINED;
		bool m_script_loaded = false;
		tmat<space::OBJECT, space::WORLD> m_transform;
		id_t m_id;
	};
} // namespace djinn

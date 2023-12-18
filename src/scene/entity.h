#pragma once
#include "pch.h"


namespace djinn
{
	class entity_manager;

	class entity
	{
		friend class entity_manager;
	public:
		entity(std::string const& fp, JSRuntime* const runtime);
		DCM(entity);
		virtual ~entity();
	public:
		void update();
		void draw();
		void draw_ui();
		tmat<space::OBJECT, space::WORLD> const& get_transform() const;
	protected:
		void inject_script(std::string const& fp, std::string const& src);
		void call_reserved(std::string const& name);
		void call_load();
		void call_unload();
		void call_init();
		void call_destroy();
		void call_main();
		void check_exception(JSValue const val, std::string const& msg);
	protected:
		JSContext* m_ctx;
		JSValue m_this = JS_UNDEFINED;
		bool m_script_loaded = false;
		tmat<space::OBJECT, space::WORLD> m_transform;
	};
} // namespace djinn

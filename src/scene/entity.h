#pragma once
#include "pch.h"


namespace djinn
{
	class entity final
	{
	public:
		entity(std::string const& fp, JSRuntime* const runtime);
		DCM(entity);
		~entity();
	public:
		void inject_script(std::string const& fp, std::string const& src);
		void update();
		void draw();
	private:
		void call_reserved(std::string const& name);
		void call_load();
		void call_unload();
		void call_init();
		void call_destroy();
		void call_main();
		void check_exception(JSValue const val, std::string const& msg);
	private:
		JSContext* m_ctx;
		JSValue m_this = JS_UNDEFINED;
		bool m_script_loaded = false;
	};
} // namespace djinn

#pragma once
#include "pch.h"

static JSValue console_log(JSContext* const ctx, JSValueConst this_val, int const argc, JSValueConst* const argv) {
	std::cout << "console.log: ";
	for (int i = 0; i < argc; ++i) {
		const char* s = JS_ToCString(ctx, argv[i]);
		std::cout << s << (i < argc - 1 ? " " : "");
		JS_FreeCString(ctx, s);
	}
	std::cout << "\n";

	return JS_UNDEFINED;
}

class entity final
{
public:
	entity(std::string const& fp, JSRuntime* const runtime);
	DCM(entity);
	~entity();
public:
	void inject_script(std::string const& fp, std::string const& src);
	void update();
private:
	void init_globals();
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

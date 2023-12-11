#pragma once
#define QJS_USE_CPP // this probably messes things up
#include <quickjs.h>
#include <string>
#include <iostream>

struct scripts_manager;

struct entity2 {
public:
	entity2(const std::string& script_fn);
	~entity2();

	void inject_script_src(const std::string& fn, const std::string& src);
	void update();

private:
	void init_js_context();
	void destroy_js_context();
	void init_global_functions();

	void call_default_func_on_this(const std::string& name);
	void handle_if_exception(JSValue maybe_ex, const std::string& where);

	JSContext* ctx = nullptr;
	bool has_script_loaded = false;
	JSValue js_this = JS_UNDEFINED;
};

extern JSRuntime* g_js_runtime;

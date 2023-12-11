#include "entity2.h"
#include "scripts_manager.h"

static JSValue console_log(JSContext* ctx, JSValueConst this_val, int argc, JSValueConst* argv) {
	std::cout << "console.log: ";
	for (int i = 0; i < argc; ++i) {
		const char* s = JS_ToCString(ctx, argv[i]);
		std::cout << s << (i < argc - 1 ? " " : "");
		JS_FreeCString(ctx, s);
	}
	std::cout << "\n";

	return JS_UNDEFINED;
}

entity2::entity2(const std::string& script_fn) {
	ctx = JS_NewContext(g_js_runtime);
	init_global_functions();
	g_all_scripts->register_entity(this, script_fn);
}

entity2::~entity2() {
	if (has_script_loaded) {
		call_default_func_on_this("__unload");
		call_default_func_on_this("__destroy");
		JS_FreeValue(ctx, js_this);
	}
	g_all_scripts->unregister_entity(this);
	JS_FreeContext(ctx);
}

void entity2::inject_script_src(const std::string& fn, const std::string& src) {
	if (has_script_loaded) {
		call_default_func_on_this("__unload");
	}

	JSValue eval_ret = JS_Eval(ctx, src.data(), src.size(), fn.c_str(), JS_EVAL_TYPE_MODULE);
	handle_if_exception(eval_ret, "entity::inject_src_src: JS_Eval(\"" + fn + "\")");
	JS_FreeValue(ctx, eval_ret);

	if (!has_script_loaded) {
		JSValue global = JS_GetGlobalObject(ctx);
		JSValue default_def = JS_GetPropertyStr(ctx, global, "__exports");
		std::cout << JS_IsUndefined(default_def);
		js_this = JS_CallConstructor(ctx, default_def, 0, nullptr);
		JS_FreeValue(ctx, default_def);
		JS_FreeValue(ctx, global);
	}
	call_default_func_on_this("__load");

	has_script_loaded = true;
}

void entity2::update() {
	call_default_func_on_this("__main");
}

void entity2::init_global_functions() {
	JSValue global = JS_GetGlobalObject(ctx);

	JSValue console = JS_NewObject(ctx);
	JS_SetPropertyStr(ctx, global, "console", console);
	JSValue log = JS_NewCFunction(ctx, console_log, "log", 1);
	JS_SetPropertyStr(ctx, console, "log", log);

	JS_FreeValue(ctx, global);
}

void entity2::call_default_func_on_this(const std::string& name)
{
	JSValue global = JS_GetGlobalObject(ctx);
	JSValue default_def = JS_GetPropertyStr(ctx, global, "__exports");
	JSValue default_proto = JS_GetPropertyStr(ctx, default_def, "prototype");
	JSValue fn = JS_GetPropertyStr(ctx, default_proto, name.c_str());
	handle_if_exception(JS_Call(ctx, fn, js_this, 0, nullptr), "call_default_func_on_this: \"" + name + "\"");
	JS_FreeValue(ctx, fn);
	JS_FreeValue(ctx, default_proto);
	JS_FreeValue(ctx, default_def);
	JS_FreeValue(ctx, global);
}

void entity2::handle_if_exception(JSValue maybe_ex, const std::string& where)
{
	if (JS_IsException(maybe_ex)) {
		JSValue ex_val = JS_GetException(ctx);
		const char* ex = JS_ToCString(ctx, ex_val);
		std::cout << "Exception occurred: " << where << ": " << ex << "\n";
		JS_FreeCString(ctx, ex);
		JS_FreeValue(ctx, ex_val);
	}
}

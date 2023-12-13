#include "pch.h"
#include "js_global.h"

namespace djinn::js::global
{
	JSValue console_log(JSContext* const ctx, JSValueConst this_val, int const argc, JSValueConst* const argv)
	{
		std::cout << "console.log: ";
		for (int i = 0; i < argc; ++i)
		{
			char const* s = JS_ToCString(ctx, argv[i]);
			std::cout << s << (i < argc - 1 ? " " : "");
			JS_FreeCString(ctx, s);
		}
		std::cout << "\n";

		return JS_UNDEFINED;
	}
} // namespace djinn::js::global
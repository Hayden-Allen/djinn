#include "pch.h"
#include "util_service.h"
#include "script/js.h"
#include "core/util.h"
#include "core/constants.h"

namespace djinn::js::util_service
{
	JSValue make_texture_path(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& path = u::to_absolute(c::base_dir::texture, js::extract_string(ctx, argv[0]));
		return JS_NewString(ctx, path.c_str());
	}
	JSValue list_files(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		std::string const& dir = u::to_absolute(js::extract_string(ctx, argv[0]));
		std::vector<std::string> filenames;
		for (auto const& entry : std::filesystem::directory_iterator(dir))
		{
			filenames.emplace_back(entry.path().string());
		}
		std::sort(filenames.begin(), filenames.end());
		JSValue ret = JS_NewArray(ctx);
		for (u32 i = 0; i < static_cast<u32>(filenames.size()); i++)
		{
			JS_SetPropertyUint32(ctx, ret, i, JS_NewString(ctx, filenames[i].c_str()));
		}
		return ret;
	}
} // namespace djinn::js::util_service

namespace djinn
{
	void util_service::init()
	{
		ASSERT(!s_instance);
		s_instance = new util_service();
	}
	void util_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "makeTexturePath", 1, js::util_service::make_texture_path);
		super::register_function(ctx, "listFiles", 1, js::util_service::list_files);
	}



	util_service::util_service() :
		haul::parent<service<util_service>>("Util")
	{}
} // namespace djinn

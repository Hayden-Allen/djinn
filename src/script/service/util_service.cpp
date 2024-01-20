#include "pch.h"
#include "util_service.h"
#include "script/js.h"
#include "core/util.h"
#include "core/constants.h"
#include "scene_service.h"

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
	JSValue vec_convert_space(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 3);
		id_t const id_from = js::extract_id(ctx, argv[0]);
		id_t const id_to = js::extract_id(ctx, argv[1]);
		vec<space::OBJECT> const& from_vec = js::extract_vec<space::OBJECT>(ctx, argv[2]);
		sptr<scene_object> from = scene_service::get_scene_object(id_from);
		sptr<scene_object> to = scene_service::get_scene_object(id_to);
		vec<space::WORLD> const world_vec = from->get_world_transform() * from_vec;
		vec<space::OBJECT> const to_vec = to->get_world_transform().invert_copy() * world_vec;
		return js::create_f32_array(ctx, 3, to_vec.e);
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
		super::register_function(ctx, "vecConvertSpace", 3, js::util_service::vec_convert_space);
	}



	util_service::util_service() :
		haul::parent<service<util_service>>("Util")
	{}
} // namespace djinn

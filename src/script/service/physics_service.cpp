#include "pch.h"
#include "physics_service.h"
#include "script/js.h"
#include "scene_service.h"

namespace djinn::js::physics_service
{
	JSValue create_physics_object(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 3);
		std::vector<f32> const& dimsarr = js::extract_f32_array(ctx, argv[0]);
		std::vector<f32> const& originarr = js::extract_f32_array(ctx, argv[1]);
		ASSERT(dimsarr.size() == 3 && originarr.size() == 3);
		f32 const mass = js::extract_f32(ctx, argv[2]);
		ASSERT(mass >= 0.f);

		btVector3 const dims(dimsarr[0], dimsarr[1], dimsarr[2]);
		btVector3 const origin(originarr[0], originarr[1], originarr[2]);

		return js::create_id(ctx, ::djinn::physics_service::get_physics_object_manager()->create(dims, origin, mass));
	}
	JSValue copy_transform_to(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 2);
		id_t const phys_id = js::extract_id(ctx, argv[0]);
		id_t const target_id = js::extract_id(ctx, argv[1]);
		sptr<physics_object> po = ::djinn::physics_service::get_physics_object_manager()->get(phys_id);
		sptr<scene_object> so = ::djinn::scene_service::get_scene_object(target_id);
		so->set_transform(po->get_transform());
		return JS_UNDEFINED;
	}
	JSValue destroy_physics_object(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv)
	{
		ASSERT(argc == 1);
		id_t const id = js::extract_id(ctx, argv[0]);
		::djinn::physics_service::get_physics_object_manager()->destroy(id);
		return JS_UNDEFINED;
	}
} // namespace djinn::js::physics_service



namespace djinn
{
	physics_service::~physics_service()
	{}



	void physics_service::init()
	{
		ASSERT(!s_instance);
		s_instance = new physics_service();
	}
	void physics_service::shutdown()
	{
		super::shutdown();
	}
	void physics_service::register_functions(JSContext* const ctx)
	{
		super::register_function(ctx, "create", 3, js::physics_service::create_physics_object);
		super::register_function(ctx, "copyTransform", 2, js::physics_service::copy_transform_to);
		super::register_function(ctx, "destroy", 1, js::physics_service::destroy_physics_object);
	}
	void physics_service::update(f32 const dt)
	{
		s_instance->m_physics_object_manager.update(dt);
	}
	physics_object_manager* physics_service::get_physics_object_manager()
	{
		return &s_instance->m_physics_object_manager;
	}



	physics_service::physics_service() :
		haul::parent<service<physics_service>>("Physics")
	{}
} // namespace djinn

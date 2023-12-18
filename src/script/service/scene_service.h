#pragma once
#include "pch.h"
#include "service.h"
#include "script/manager/entity_manager.h"

namespace djinn
{
	namespace js::scene_service
	{
		extern JSValue load_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue load_camera(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue get_entity_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue set_entity_pos(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
		extern JSValue destroy_entity(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
	} // namespace js::scene_service

	class scene_service final : public haul::parent<service<scene_service>>
	{
		friend class super;
	public:
		DCM(scene_service);
	public:
		static void init();
		static void register_functions(JSContext* const ctx);
		static entity_manager* get_entity_manager();
	private:
		entity_manager m_entity_manager;
	private:
		scene_service();
	};
} // namespace djinn

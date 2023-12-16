#pragma once
#include "pch.h"
#include "service.h"

namespace djinn
{
	namespace js::render_service
	{
		extern JSValue draw(JSContext* const ctx, JSValueConst this_val, s32 const argc, JSValueConst* const argv);
	}

	class render_service final : public haul::parent<service<render_service>>
	{
		friend class super;
	public:
		DCM(render_service);
	public:
		static void init(optr<mgl::context>& context);
		static void register_functions(JSContext* const ctx);
		static sptr<mgl::context> get_context();
	private:
		sptr<mgl::context> m_context;
	private:
		render_service(optr<mgl::context>& context);
	};
} // namespace djinn

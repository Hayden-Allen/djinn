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
		static void init(mgl::context* const context);
		static void register_functions(JSContext* const ctx);
		mgl::context* const get_context();
	private:
		mgl::context* const m_context;
	private:
		render_service(mgl::context* const context);
	};
}

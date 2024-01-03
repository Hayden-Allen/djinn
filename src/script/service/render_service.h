#pragma once
#include "pch.h"
#include "service.h"

namespace djinn
{
	class render_service final : public haul::parent<service<render_service>>
	{
		friend super;
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

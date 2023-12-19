#pragma once
#include "pch.h"
#include "service.h"

namespace djinn
{
	class input_service final : public haul::parent<service<input_service>>
	{
		friend class super;
	public:
		DCM(input_service);
	public:
		static void init(sptr<mgl::context> const& context);
		static void register_functions(JSContext* const ctx);
		static sptr<mgl::context> get_context();
	private:
		sptr<mgl::context> m_context;
	private:
		input_service(sptr<mgl::context> const& context);
	};
} // namespace djinn

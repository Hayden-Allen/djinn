#pragma once
#include "pch.h"
#include "service.h"

namespace djinn
{
	class util_service final : public haul::parent<service<util_service>>
	{
		friend class super;
	public:
		DCM(util_service);
	public:
		static void init();
		static void register_functions(JSContext* const ctx);
	private:
		util_service();
	};
} // namespace djinn

#pragma once
#include "pch.h"

namespace djinn
{
	class power_loader
	{
	public:
		power_loader() {}
		DCM(power_loader);
	public:
		virtual std::vector<id_t> load_all(mgl::input_file* const in) = 0;
	};
} // namespace djinn

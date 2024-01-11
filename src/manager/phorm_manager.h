#pragma once
#include "pch.h"
#include "manager.h"
#include "power_loader.h"
#include "scene/phorm.h"

namespace djinn
{
	class shaders;

	class phorm_manager final : public ref_counted_manager<phorm>, public power_loader
	{
	public:
		phorm_manager();
		DCM(phorm_manager);
	public:
		std::vector<id_t> load_all(mgl::input_file* const in) override;
		void destroy(id_t const id) override;
	};
} // namespace djinn

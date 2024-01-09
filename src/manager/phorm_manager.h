#pragma once
#include "pch.h"
#include "manager.h"
#include "scene/phorm.h"

namespace djinn
{
	class shaders;

	class phorm_manager final : public ref_counted_file_manager<phorm>
	{
	public:
		phorm_manager();
		DCM(phorm_manager);
	public:
		std::vector<id_t> load_all(std::string const& fp);
		id_t load(std::string const& fp) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void destroy(id_t const id) override;
	private:
		haul::bdumap<id_t, std::string> m_id2afp;
	};
} // namespace djinn

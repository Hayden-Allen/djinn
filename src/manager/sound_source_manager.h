#pragma once
#include "pch.h"
#include "manager.h"
#include "asset/sound_source.h"

namespace djinn
{
	class sound_source;

	class sound_source_manager final : public ref_counted_file_manager<sound_source>
	{
	public:
		sound_source_manager();
		~sound_source_manager();
		DCM(sound_source_manager);
	public:
		void configure();
		void unconfigure(bool needs_reload_all = true);
		ma_resource_manager* get_resource_manager();
	public:
		id_t load(std::string const& fp) override;
		void destroy(id_t const id) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
	private:
		ma_resource_manager m_resource_manager;
		bool m_resource_manager_ready = false;
		haul::bdumap<id_t, std::string> m_id2afp;
	private:
		void reload_all();
	};
} // namespace djinn

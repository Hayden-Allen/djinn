#pragma once
#include "pch.h"
#include "manager/manager.h"

namespace djinn
{
	class texture_manager final : public ref_counted_file_manager<texture>
	{
	public:
		texture_manager();
		DCM(texture_manager);
	public:
		id_t create(u32 const width, u32 const height, texture_options const& options);
		std::pair<std::vector<id_t>, std::vector<sptr<texture>>> load_xport(mgl::input_file* const in);
		id_t load(std::string const& fp) override;
		id_t load(std::string const& fp, texture_options const& options);
		void destroy(id_t const id) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void update(id_t const id, std::vector<u8> const& subpixels, texture_options const& options);
		void update(id_t const id, std::vector<u8> const& subpixels);
		void bind(id_t const id, u32 const slot);
	private:
		haul::bdumap<id_t, std::string> m_id2afp;
		std::unordered_map<id_t, texture_options> m_id2options;
	private:
		id_t find_existing(std::string const& afp, texture_options const& options);
	};
} // namespace djinn

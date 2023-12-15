#pragma once
#include "pch.h"
#include "manager.h"

namespace djinn
{
	class texture_manager final : public manager<texture2d_rgba_u8>
	{
	public:
		texture_manager();
		DCM(texture_manager);
	public:
		id_t create(u32 const width, u32 const height, texture_options const& options);
		id_t load(std::string const& fp) override;
		void destroy(id_t const id) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void update(id_t const id, std::vector<u8> const& subpixels, texture_options const& options);
		void bind(id_t const id, u32 const slot);
	};
} // namespace djinn

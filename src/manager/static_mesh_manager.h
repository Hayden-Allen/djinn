#pragma once
#include "pch.h"
#include "manager.h"
#include "asset/static_mesh.h"

namespace djinn
{
	class static_mesh_manager final : public ref_counted_file_manager<static_mesh>
	{
	public:
		static_mesh_manager();
		DCM(static_mesh_manager);
		~static_mesh_manager();
	public:
		id_t load(std::string const& fp) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void destroy(id_t const id) override;
	private:
		haul::bdumap<id_t, std::string> m_id2afp;
		std::unordered_map<std::string, m3d_t*> m_afp2raw;
	private:
		m3d_t* get_or_load(std::string const& afp);
		m3d_t* load_raw(std::string const& afp);
	};
} // namespace djinn

#pragma once
#include "pch.h"
#include "manager.h"
#include "scene/mesh_instance.h"

namespace djinn
{
	class mesh;
	class shaders;

	class mesh_instance_manager final : public haul::parent<ref_counted_manager<mesh_instance>>
	{
	public:
		mesh_instance_manager();
		DCM(mesh_instance_manager);
	public:
		id_t create(sptr<mesh> mesh, sptr<shaders> shaders);
		id_t load(std::string const& fp) override;
		void destroy(id_t const id) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void free_all() override;
	};
} // namespace djinn
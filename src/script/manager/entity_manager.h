#pragma once
#include "pch.h"
#include "manager.h"
#include "scene/entity.h"
#include "scene/camera_entity.h"

namespace djinn
{
	class entity_manager final : public manager<entity>
	{
	public:
		entity_manager(JSRuntime* const runtime);
		DCM(entity_manager);
	public:
		id_t load(std::string const& fp) override;
		id_t load_camera(std::string const& fp);
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void destroy(id_t const id) override;
		camera_entity* get_camera(id_t const id);
		template<typename FN>
		void for_each(FN const& fn)
		{
			for (auto& pair : m_objects)
				fn(sptr<entity>(pair.second), pair.first);
		}
		template<typename FN>
		void for_each(FN const& fn) const
		{
			for (auto const& pair : m_objects)
				fn(sptr<entity>(pair.second), pair.first);
		}
	private:
		JSRuntime* const m_runtime;
		std::unordered_map<std::string, std::string> m_afp2src;
		haul::bdumap<id_t, std::string> m_id2afp;
		std::unordered_set<id_t> m_cameras;
	private:
		void inject_script(std::string const& afp);
		void load_file(std::string const& afp);
		id_t load_base(entity* const e, std::string const& afp);
	};
} // namespace djinn

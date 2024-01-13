#pragma once
#include "pch.h"
#include "mesh_instance.h"
#include "asset/mesh/animated_mesh.h"

namespace djinn
{
	class animated_mesh_instance final : public mesh_instance
	{
	public:
		animated_mesh_instance(id_t const id, sptr<mesh> const& mesh, wptr<shaders> const& batch_shaders);
		DCM(animated_mesh_instance);
	public:
		bool is_animated() const override;
		void set_action(std::string const& name, f32 const speed);
		std::vector<tmat<space::OBJECT, space::WORLD>> const& get_pose() const;
		tmat<space::OBJECT, space::WORLD> get_bone_transform(std::string const& name);
	private:
		animated_mesh::action const* m_current_action;
		f32 m_action_start, m_action_speed;
		mutable std::vector<tmat<space::OBJECT, space::WORLD>> m_last_pose;
	};
} // namespace djinn

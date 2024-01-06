#pragma once
#include "pch.h"
#include "mesh_instance.h"
#include "asset/animated_mesh.h"

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
		m3db_t* get_pose() const;
	private:
		animated_mesh::action const* m_current_action;
		f32 m_action_start, m_action_speed;
	};
} // namespace djinn

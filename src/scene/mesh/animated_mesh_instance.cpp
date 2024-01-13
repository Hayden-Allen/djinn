#include "pch.h"
#include "animated_mesh_instance.h"
#include "script/service/render_service.h"

namespace djinn
{
	animated_mesh_instance::animated_mesh_instance(id_t const id, sptr<mesh> const& mesh, wptr<shaders> const& batch_shaders) :
		mesh_instance(id, mesh, batch_shaders),
		m_current_action(nullptr),
		m_action_start(0)
	{
		ASSERT(mesh->is_animated());
		set_action("bind", 1.f);
	}



	bool animated_mesh_instance::is_animated() const
	{
		return true;
	}
	void animated_mesh_instance::set_action(std::string const& name, f32 const speed)
	{
		sptr<animated_mesh> am = m_mesh;
		ASSERT(am->m_actions.contains(name));
		m_current_action = &am->m_actions.at(name);
		m_action_start = render_service::get_context()->time.now;
		m_action_speed = speed;
	}
	std::vector<tmat<space::OBJECT, space::WORLD>> const& animated_mesh_instance::get_pose() const
	{
		f32 const delta = render_service::get_context()->time.now - m_action_start;
		sptr<animated_mesh> am = m_mesh;
		m_last_pose = am->get_pose(m_current_action->id, delta * m_action_speed);
		return m_last_pose;
	}
	tmat<space::OBJECT, space::WORLD> animated_mesh_instance::get_bone_transform(std::string const& name)
	{
		if (m_last_pose.empty())
			return tmat<space::OBJECT, space::WORLD>();
		sptr<animated_mesh> am = m_mesh;
		return m_last_pose.at(am->get_bone_index(name));
	}
} // namespace djinn

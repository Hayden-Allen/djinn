#include "pch.h"
#include "animated_mesh_instance.h"
#include "script/service/render_service.h"
#include "core/m3d.h"

namespace djinn
{
	animated_mesh_instance::animated_mesh_instance(id_t const id, sptr<mesh> const& mesh, wptr<shaders> const& batch_shaders) :
		mesh_instance(id, mesh, batch_shaders),
		m_current_action(nullptr),
		m_action_start(0)
	{
		ASSERT(mesh->is_animated());
		set_action("bind", 1.f);

		sptr<animated_mesh> am = m_mesh;
		m_skel.resize(am->get_num_bones() * 7);
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
	m3db_t const* animated_mesh_instance::get_pose() const
	{
		f32 const delta = render_service::get_context()->time.now - m_action_start;
		sptr<animated_mesh> am = m_mesh;
		m3db_t const* const bones = am->get_pose(m_current_action->id, delta * m_action_speed);

		m3d_t const* const raw = am->m_raw;
		for (u32 i = 0; i < am->m_raw->numbone; i++)
		{
			m_skel[i * 7 + 0] = raw->vertex[bones[i].pos].x;
			m_skel[i * 7 + 1] = raw->vertex[bones[i].pos].y;
			m_skel[i * 7 + 2] = raw->vertex[bones[i].pos].z;
			m_skel[i * 7 + 3] = raw->vertex[bones[i].ori].x;
			m_skel[i * 7 + 4] = raw->vertex[bones[i].ori].y;
			m_skel[i * 7 + 5] = raw->vertex[bones[i].ori].z;
			m_skel[i * 7 + 6] = raw->vertex[bones[i].ori].w;
			if (bones[i].parent != M3D_UNDEF)
			{
				m3d::quat_mul_quat(&m_skel[i * 7 + 3], &m_skel[bones[i].parent * 7 + 3]);
				m3d::vec3_mul_quat(&m_skel[i * 7], &m_skel[bones[i].parent * 7 + 3]);
				m_skel[i * 7 + 0] += m_skel[bones[i].parent * 7 + 0];
				m_skel[i * 7 + 1] += m_skel[bones[i].parent * 7 + 1];
				m_skel[i * 7 + 2] += m_skel[bones[i].parent * 7 + 2];
			}
		}
		return bones;
	}
	point<space::WORLD> animated_mesh_instance::get_bone_pos(std::string const& name)
	{
		sptr<animated_mesh> am = m_mesh;
		u32 const idx = am->get_bone_index(name);
		point<space::OBJECT> po(
			m_skel[idx * 7 + 0],
			m_skel[idx * 7 + 1],
			m_skel[idx * 7 + 2]);
		return po.transform_copy(get_world_transform());
	}
} // namespace djinn

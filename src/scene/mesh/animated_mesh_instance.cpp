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
		animated_mesh::action* const new_action = &am->m_actions.at(name);
		if (m_current_action != new_action)
		{
			m_current_action = new_action;
			m_action_start = render_service::get_context()->time.now;
			m_action_speed = speed;
		}
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
	tmat<space::OBJECT, space::WORLD> animated_mesh_instance::get_bone_transform(std::string const& name) const
	{
		sptr<animated_mesh> am = m_mesh;
		u32 const idx = am->get_bone_index(name);
		f32 const x = m_skel[idx * 7 + 0];
		f32 const y = m_skel[idx * 7 + 1];
		f32 const z = m_skel[idx * 7 + 2];
		f32 const q0 = m_skel[idx * 7 + 3];
		f32 const q1 = m_skel[idx * 7 + 4];
		f32 const q2 = m_skel[idx * 7 + 5];
		f32 const q3 = m_skel[idx * 7 + 6];

		f32 mat[16] = { 0 };
		/*mat[0] = 2 * (q0 * q0 + q1 * q1) - 1;
		mat[1] = 2 * (q1 * q2 - q0 * q3);
		mat[2] = 2 * (q1 * q3 + q0 * q2);
		mat[4] = 2 * (q1 * q2 + q0 * q3);
		mat[5] = 2 * (q0 * q0 + q2 * q2) - 1;
		mat[6] = 2 * (q2 * q3 - q0 * q1);
		mat[8] = 2 * (q1 * q3 - q0 * q2);
		mat[9] = 2 * (q2 * q3 + q0 * q1);
		mat[10] = 2 * (q0 * q0 + q3 * q3) - 1;*/
		mat[0] = 2 * (q0 * q0 + q1 * q1) - 1;
		mat[4] = 2 * (q1 * q2 - q0 * q3);
		mat[8] = 2 * (q1 * q3 + q0 * q2);
		mat[1] = 2 * (q1 * q2 + q0 * q3);
		mat[5] = 2 * (q0 * q0 + q2 * q2) - 1;
		mat[9] = 2 * (q2 * q3 - q0 * q1);
		mat[2] = 2 * (q1 * q3 - q0 * q2);
		mat[6] = 2 * (q2 * q3 + q0 * q1);
		mat[10] = 2 * (q0 * q0 + q3 * q3) - 1;
		mat[12] = x;
		mat[13] = y;
		mat[14] = z;
		return mat;
	}
} // namespace djinn

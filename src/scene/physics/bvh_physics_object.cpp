#include "pch.h"
#include "bvh_physics_object.h"
#include "scene/xport/phorm.h"

namespace djinn
{
	bvh_physics_object::bvh_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, sptr<phorm> const& phorm) :
		physics_object(id, world)
	{
		if (phorm->get_vertex_count() != 0)
		{
			m_iva = new btTriangleIndexVertexArray();

			// create separate btIndexedMesh for each material used by this phorm
			// (they all end up as one BVH shape anyway)
			auto const& ros = phorm->get_render_objects();
			for (auto const& pair : ros)
			{
				btIndexedMesh mesh;

				static_retained_render_object const& ro = pair.second;
				auto const& vao = ro.get_vertex_array();
				mesh.m_triangleIndexStride = 3 * sizeof(u32);
				mesh.m_triangleIndexBase = (unsigned char const*)ro.get_indices().get_data();
				mesh.m_numTriangles = ro.get_index_count() / 3;
				mesh.m_numVertices = vao.get_vertex_count();
				mesh.m_vertexBase = (unsigned char const*)vao.get_vertices().get_data();
				mesh.m_vertexStride = vao.get_stride();

				m_iva->addIndexedMesh(mesh);
			}
			m_shape = new btBvhTriangleMeshShape(m_iva.get(), true, true);
		}
		else
		{
			m_shape = new btEmptyShape();
		}

		btTransform mat;
		mat.setIdentity();
		m_motion = new btDefaultMotionState(mat);

		btRigidBody::btRigidBodyConstructionInfo info(0.f, m_motion.get(), m_shape.get(), { 0, 0, 0 });
		m_rb = new btRigidBody(info);
		m_rb->setActivationState(DISABLE_DEACTIVATION);
		m_world->addRigidBody(m_rb.get());

		bind_to_bullet();
	}
	bvh_physics_object::~bvh_physics_object()
	{
		m_iva.free();
	}



	bool bvh_physics_object::is_bvh() const
	{
		return true;
	}
	// none of these operatinos are valid on a bvh triangle mesh
	void bvh_physics_object::collide_and_slide(vec<space::OBJECT> const& vel, f32 const dt, vec<space::WORLD> const& threshold)
	{
		ASSERT(false);
	}
	void bvh_physics_object::apply_impulse(vec<space::OBJECT> const& force)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_velocity(f32 const x, f32 const y, f32 const z)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_velocity_x(f32 const x)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_velocity_y(f32 const y)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_velocity_z(f32 const z)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_velocity_world(f32 const x, f32 const y, f32 const z)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_velocity_x_world(f32 const x)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_velocity_y_world(f32 const y)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_velocity_z_world(f32 const z)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_damping(f32 const d)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_angular_velocity(f32 const x, f32 const y, f32 const z)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_angular_damping(f32 const d)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_angular_factor(f32 const x, f32 const y, f32 const z)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_max_speed(u32 const index, f32 const max)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_gravity(vec<space::WORLD> const& force)
	{
		ASSERT(false);
	}
	void bvh_physics_object::set_kinematic(bool const is_kinematic)
	{
		ASSERT(false);
	}
} // namespace djinn

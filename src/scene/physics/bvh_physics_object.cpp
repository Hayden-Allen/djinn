#include "pch.h"
#include "bvh_physics_object.h"

namespace djinn
{
	bvh_physics_object::bvh_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, static_retained_render_object* const ro) :
		physics_object(id, world)
	{
		const s32 num_triangles = ro->get_index_count() / 3;
		s32* index_base = (s32*)ro->get_indices().get_data();
		const s32 index_stride = 3 * sizeof(u32);

		auto& vao = ro->get_vertex_array();
		const s32 num_vertices = vao.get_vertex_count();
		f32* vertex_base = vao.get_vertices().get_data();
		const s32 vertex_stride = vao.get_stride();

		m_iva = new btTriangleIndexVertexArray(num_triangles, index_base, index_stride, num_vertices, vertex_base, vertex_stride);
		m_shape = new btBvhTriangleMeshShape(m_iva.get(), true, true);

		btTransform mat;
		mat.setIdentity();
		// mat.setOrigin(origin);
		m_motion = new btDefaultMotionState(mat);

		btRigidBody::btRigidBodyConstructionInfo info(0.f, m_motion.get(), m_shape.get(), { 0, 0, 0 });
		m_rb = new btRigidBody(info);
		m_rb->setActivationState(DISABLE_DEACTIVATION);
		m_world->addRigidBody(m_rb.get());
	}
	bvh_physics_object::~bvh_physics_object()
	{
		m_iva.free();
	}
} // namespace djinn

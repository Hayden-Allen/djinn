#include "pch.h"
#include "bvh_physics_object.h"
#include "scene/xport/phorm.h"

namespace djinn
{

	bvh_physics_object::bvh_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, sptr<phorm> const& phorm) :
		physics_object(id, world)
	{
		m_iva = new btTriangleIndexVertexArray();

		// create separate btIndexedMesh for each material used by this phorm
		// (they all end up as one BVH shape anyway)
		auto const& ros = phorm->get_render_objects();
		m_vbo.reserve(ros.size());
		for (auto const& pair : ros)
		{
			btIndexedMesh mesh;

			static_retained_render_object const& ro = pair.second;
			auto const& vao = ro.get_vertex_array();

			// dedup on position
			std::vector<power_vertex>& vbo = m_vbo.emplace_back();
			std::unordered_map<power_vertex, u32> vert2idx;
			std::vector<u32>& ibo = m_ibo.emplace_back();
			vbo.reserve(vao.get_vertex_count());
			ibo.reserve(ro.get_index_count());
			f32 const* const raw = vao.get_vertices().get_data();
			u32 const stride = vao.get_stride() / sizeof(f32);
			for (u32 i = 0; i < ro.get_index_count(); i++)
			{
				u32 const idx = ro.get_indices().get_data()[i];
				power_vertex cur(&raw[idx * stride]);
				auto const& it = vert2idx.find(cur);
				if (it != vert2idx.end())
				{
					// printf("(%.6f %.6f %.6f) == (%.6f %.6f %.6f)\n", cur.pos[0], cur.pos[1], cur.pos[2], it->first.pos[0], it->first.pos[1], it->first.pos[2]);
					ibo.push_back(it->second);
				}
				else
				{
					u32 const idx = (u32)vbo.size();
					vbo.push_back(cur);
					vert2idx.insert({ cur, idx });
					ibo.push_back(idx);
				}
			}
			vbo.shrink_to_fit();
			ibo.shrink_to_fit();

			/*mesh.m_triangleIndexStride = 3 * sizeof(u32);
			mesh.m_triangleIndexBase = (unsigned char const*)ro.get_indices().get_data();
			mesh.m_numTriangles = ro.get_index_count() / 3;
			mesh.m_numVertices = vao.get_vertex_count();
			mesh.m_vertexBase = (unsigned char const*)vao.get_vertices().get_data();
			mesh.m_vertexStride = vao.get_stride();*/
			mesh.m_triangleIndexStride = 3 * sizeof(u32);
			mesh.m_triangleIndexBase = (unsigned char const*)m_ibo.back().data();
			mesh.m_numTriangles = (u32)m_ibo.back().size() / 3;
			mesh.m_numVertices = (u32)m_vbo.back().size();
			mesh.m_vertexBase = (unsigned char const*)m_vbo.back().data();
			mesh.m_vertexStride = sizeof(power_vertex);

			m_iva->addIndexedMesh(mesh);
		}
		m_shape = new btBvhTriangleMeshShape(m_iva.get(), true, true);

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
} // namespace djinn

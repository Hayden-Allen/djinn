#include "pch.h"
#include "physics_debug_drawer.h"
#include "script/service/render_service.h"

namespace djinn
{
	static char const* vert_src = R"(
		#version 410 core
		layout(location = 0) in vec3 i_pos;
		layout(location = 1) in vec3 i_col;
		uniform mat4 u_vp;
		out vec3 v_col;
		void main() {
			gl_Position = u_vp * vec4(i_pos, 1);
			v_col = i_col;
		}
	)";
	static char const* frag_src = R"(
		#version 410 core
		layout(location = 0) out vec4 o_color;
		in vec3 v_col;
		void main() {
			o_color = vec4(v_col, 1);
		}
	)";

	physics_debug_drawer::physics_debug_drawer() :
		m_shaders(vert_src, frag_src, true),
		m_vao(nullptr, s_max_lines, { { GL_FLOAT, { 3, 3 } } }),
		m_valid(0)
	{}



	void physics_debug_drawer::drawLine(btVector3 const& from, btVector3 const& to, btVector3 const& color)
	{
		ASSERT(m_valid < s_max_lines);
		m_lines[m_valid] = { from, to, color };
		m_valid++;
	}
	void physics_debug_drawer::clearLines()
	{
		m_valid = 0;
	}
	void physics_debug_drawer::flushLines()
	{
		m_vao.get_vertices().update(m_lines.data(), m_valid, 0);
		glDepthFunc(GL_LEQUAL);
		render_service::get_context()->draw_lines_arrays(m_vao, m_shaders, 3.f);
		glDepthFunc(GL_LESS);
	}
	void physics_debug_drawer::drawContactPoint(btVector3 const& PointOnB, btVector3 const& normalOnB, btScalar distance, int lifeTime, btVector3 const& color)
	{}
	void physics_debug_drawer::reportErrorWarning(char const* warningString)
	{
		printf("physics_debug_drawer: %s\n", warningString);
	}
	void physics_debug_drawer::draw3dText(btVector3 const& location, char const* textString)
	{}
	void physics_debug_drawer::setDebugMode(int debugMode)
	{
		__debugbreak();
	}
	int physics_debug_drawer::getDebugMode() const
	{
		return DBG_DrawWireframe;
	}
	void physics_debug_drawer::set_camera(mat<space::WORLD, space::CLIP> const& vp)
	{
		m_shaders.uniform_mat4("u_vp", vp.e);
	}
} // namespace djinn

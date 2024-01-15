#pragma once
#include "pch.h"

namespace djinn
{
	class physics_debug_drawer final : public btIDebugDraw
	{
		struct line
		{
			btVector3 from, to, color;
		};
	public:
		physics_debug_drawer();
		DCM(physics_debug_drawer);
	public:
		void drawLine(btVector3 const& from, btVector3 const& to, btVector3 const& color) override;
		void clearLines() override;
		void flushLines() override;
		void drawContactPoint(btVector3 const& PointOnB, btVector3 const& normalOnB, btScalar distance, int lifeTime, btVector3 const& color) override;
		void reportErrorWarning(char const* warningString) override;
		void draw3dText(btVector3 const& location, char const* textString) override;
		void setDebugMode(int debugMode) override;
		int getDebugMode() const override;
		void set_camera(mat<space::WORLD, space::CLIP> const& vp);
	private:
		static constexpr u64 s_max_lines = 65536;
		static constexpr u32 s_floats_per_line = sizeof(line) / sizeof(float);
	private:
		mgl::shaders m_shaders;
		dynamic_structured_vertex_array<line> m_vao;
		std::array<line, s_max_lines> m_lines;
		u32 m_valid;
	};
} // namespace djinn
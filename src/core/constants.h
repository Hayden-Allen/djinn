#pragma once
#include "pch.h"

namespace djinn::c
{
	namespace base_dir
	{
		static std::string const mesh = "../../../../cwd/res/mesh";
		static std::string const script = "../../../../cwd/res/scripts/out";
		static std::string const shader = "../../../../cwd/res/glsl";
		static std::string const texture = "../../../../cwd/res/texture";
		static std::string const font = "../../../../cwd/res/font";
		static std::string const sound = "../../../../cwd/res/sound";
	} // namespace base_dir
	namespace uniform
	{
		static std::string const view_mat = "u_view";
		static std::string const proj_mat = "u_proj";
		static std::string const vp_mat = "u_vp";
		static std::string const vpr_mat = "u_vpr";
		static std::string const instanced_transforms_block = "u_instanced_transforms";
	} // namespace uniform
} // namespace djinn::c

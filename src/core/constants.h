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
		static std::string const view_mat = "d_view";
		static std::string const proj_mat = "d_proj";
		static std::string const vp_mat = "d_vp";
		static std::string const vpr_mat = "d_vpr";
		static std::string const instance_block_type = "d_instance_block";
		static std::string const instances_uniform = "d_instances";
		static std::string const instance_struct = "d_instance_struct";
		static std::string const instance_transform = "d_transform";
	} // namespace uniform
} // namespace djinn::c

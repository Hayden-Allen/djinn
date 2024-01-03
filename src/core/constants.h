#pragma once
#include "pch.h"

namespace djinn::c
{
	namespace base_dir
	{
#ifdef _WIN32
		static std::string const mesh = "../../../../cwd/res/mesh";
		static std::string const script = "../../../../cwd/res/scripts/out";
		static std::string const shader = "../../../../cwd/res/glsl";
		static std::string const texture = "../../../../cwd/res/texture";
		static std::string const font = "../../../../cwd/res/font";
		static std::string const sound = "../../../../cwd/res/sound";
#else
		static std::string const mesh = "../../cwd/res/mesh";
		static std::string const script = "../../cwd/res/scripts/out";
		static std::string const shader = "../../cwd/res/glsl";
		static std::string const texture = "../../cwd/res/texture";
		static std::string const font = "../../cwd/res/font";
		static std::string const sound = "../../cwd/res/sound";
#endif
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
	namespace shader
	{
		static u32 num_ubos = 12;
		static u32 ubo_size_bytes = 16384;
		namespace type
		{
			static std::string const basic = "basic";
			static std::string const animated = "animated";
			static std::string const custom = "custom";
		} // namespace type
		namespace macro
		{
			static std::string const type = "TYPE";
			static std::string const instance_field = "FIELD";
		} // namespace macro
	}	  // namespace shader
} // namespace djinn::c

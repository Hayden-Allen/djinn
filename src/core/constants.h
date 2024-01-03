#pragma once
#include "pch.h"

namespace djinn::c
{
	namespace base_dir
	{
#ifdef _WIN32
		static std::string const root = "../../../../";
#else
		static std::string const root = "../../";
#endif
		static std::string const mesh = root + "cwd/res/mesh";
		static std::string const script = root + "cwd/res/scripts/out";
		static std::string const shader = root + "cwd/res/glsl";
		static std::string const texture = root + "cwd/res/texture";
		static std::string const font = root + "cwd/res/font";
		static std::string const sound = root + "cwd/res/sound";
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
		static std::string const instance_model_mat = "d_model";
		static std::string const instance_normal_mat = "d_normal";
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

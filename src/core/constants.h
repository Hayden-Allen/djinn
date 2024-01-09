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
		static std::string const phorm = root + "cwd/res/phorm";
	} // namespace base_dir
	namespace shader
	{
		static constexpr u32 num_ubos = 12;
		static constexpr u32 ubo_size_bytes = 16384;
		static constexpr u32 num_vertex_bones = M3D_NUMBONE;
		static constexpr u32 num_bones = 68;
		static std::string const instance_struct = "d_instance_struct";
		static std::string const instance_model_mat = "d_model";
		static std::string const instance_normal_mat = "d_normal";
		static std::string const instance_bones = "d_bones";
		static std::string const vertex_pos = "d_pos";
		static std::string const vertex_norm = "d_norm";
		static std::string const vertex_tex = "d_tex";
		static std::string const vertex_bone_index = "d_index";
		static std::string const vertex_bone_weight = "d_weight";
		namespace type
		{
			static std::string const STATIC = "STATIC";
			static std::string const ANIMATED = "ANIMATED";
			static std::string const CUSTOM = "CUSTOM";
		} // namespace type
		namespace macro
		{
			static std::string const type = "TYPE";
			static std::string const instance_field = "FIELD";
		} // namespace macro
	}	  // namespace shader
	namespace uniform
	{
		static std::string const time = "d_time";
		static std::string const view_mat = "d_view";
		static std::string const proj_mat = "d_proj";
		static std::string const vp_mat = "d_vp";
		static std::string const vpr_mat = "d_vpr";
		static std::string const instance_block_type = "d_instance_block";
		static std::string const instances = "d_instances";
		static std::string const instance_block_name[::djinn::c::shader::num_ubos] = {
			"d_instance_block[0]",
			"d_instance_block[1]",
			"d_instance_block[2]",
			"d_instance_block[3]",
			"d_instance_block[4]",
			"d_instance_block[5]",
			"d_instance_block[6]",
			"d_instance_block[7]",
			"d_instance_block[8]",
			"d_instance_block[9]",
			"d_instance_block[10]",
			"d_instance_block[11]",
		};
	} // namespace uniform
} // namespace djinn::c

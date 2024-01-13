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
		static std::string const xport = root + "cwd/res/xport";
	} // namespace base_dir
	namespace shader
	{
		static constexpr u32 num_ubos = 12;
		static constexpr u32 num_batch_ubos = 11;
		static constexpr u32 ubo_size_bytes = 16384;
		static constexpr u32 num_vertex_bones = M3D_NUMBONE;
		static constexpr u32 num_bones = 68;
		static constexpr u32 num_phorm_textures = 4;
		// number of lights is also stored in the ubo, so -1
		static constexpr u32 num_lights = (ubo_size_bytes / sizeof(mgl::light)) - 1;
		static std::string const instance_struct = "d_instance_struct";
		static std::string const light_struct = "d_light_struct";
		static std::string const instance_model_mat = "d_model";
		static std::string const instance_normal_mat = "d_normal";
		static std::string const instance_bones = "d_bones";
		static std::string const vertex_pos = "d_pos";
		static std::string const vertex_norm = "d_norm";
		static std::string const vertex_tex = "d_tex";
		static std::string const vertex_bone_index = "d_index";
		static std::string const vertex_bone_weight = "d_weight";
		static std::string const vertex_uvs[4] = {
			"d_uv0",
			"d_uv1",
			"d_uv2",
			"d_uv3"
		};
		static std::string const vertex_texture_weights = "d_weights";
		static std::string const vertex_color = "d_rgba";
		static std::string const light_ambient = "d_ambient";
		static std::string const light_diffuse = "d_diffuse";
		static std::string const light_specular = "d_specular";
		namespace type
		{
			static std::string const STATIC = "STATIC";
			static std::string const ANIMATED = "ANIMATED";
			static std::string const PHORM = "PHORM";
			static std::string const CUSTOM = "CUSTOM";
		} // namespace type
		namespace macro
		{
			static std::string const type = "TYPE";
			static std::string const instance_field = "FIELD";
			static std::string const light_sum = "LIGHT_SUM";
		} // namespace macro
	}	  // namespace shader
	namespace uniform
	{
		static constexpr u64 light_ubo_index = ::djinn::c::shader::num_ubos - 1;
		static std::string const light_block_name = "d_lights";
		static std::string const light_block_type = "d_light";
		static std::string const phorm_textures[::djinn::c::shader::num_phorm_textures] = {
			"d_phorm_tex0",
			"d_phorm_tex1",
			"d_phorm_tex2",
			"d_phorm_tex3",
		};
		static std::string const phorm_use_lighting = "d_use_lighting";
		static std::string const time = "d_time";
		static std::string const view_mat = "d_view";
		static std::string const proj_mat = "d_proj";
		static std::string const vp_mat = "d_vp";
		static std::string const vpr_mat = "d_vpr";
		static std::string const cam_pos = "d_cam_pos";
		static std::string const model_mat = "d_model";
		static std::string const normal_mat = "d_normal";
		static std::string const instance_block_type = "d_instance_block";
		static std::string const instances = "d_instances";
		static std::string const instance_block_name[::djinn::c::shader::num_batch_ubos] = {
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
		};
	} // namespace uniform
} // namespace djinn::c

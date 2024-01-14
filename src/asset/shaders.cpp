#include "pch.h"
#include "shaders.h"
#include "core/constants.h"
#include "script/service/render_service.h"

namespace djinn
{
	shaders::shaders(std::string const& vert_afp, std::string const& frag_afp) :
		m_vert_afp(vert_afp),
		m_frag_afp(frag_afp),
		m_type(shader_type::NONE)
	{
		init(vert_afp, frag_afp);
	}



	std::vector<shaders::instance_field> const& shaders::get_instance_fields() const
	{
		return m_instance_fields;
	}
	u32 shaders::get_base_offset_bytes() const
	{
		ASSERT(m_type != shader_type::PHORM);
		return s_base_offset_bytes[(u64)m_type];
	}
	void shaders::init(std::string const& vert_afp, std::string const& frag_afp)
	{
		std::string const& vert_src = preprocess_vert(vert_afp);
		std::string const& frag_src = preprocess_frag(frag_afp);
		super::init(vert_src, frag_src, true);

		uniform_block_binding(c::uniform::light_block_name, c::uniform::light_ubo_index);
	}
	bool shaders::uniform_block_binding(std::string const& name, s32 const slot) const
	{
		if (!super::uniform_block_binding(name, slot))
		{
			// printf("[WARNING] shaders(%s, %s):\n\tuniform_block_binding: Uniform block '%s' does not exist\n", m_vert_afp.c_str(), m_frag_afp.c_str(), name.c_str());
			return false;
		}
		return true;
	}



	std::vector<std::string> shaders::extract_args(std::string const& line)
	{
		u64 const start = line.find_first_of('(') + 1;
		u64 const end = line.find_last_of(')');
		std::vector<std::string> args = u::split(line.substr(start, end - start), ",");
		for (std::string& arg : args)
			arg = u::trim(arg);
		return args;
	}
	shaders::shader_type shaders::string_to_shader_type(std::string const& type)
	{
		if (u::iequals(type, c::shader::type::STATIC))
			return shader_type::STATIC;
		else if (u::iequals(type, c::shader::type::ANIMATED))
			return shader_type::ANIMATED;
		else if (u::iequals(type, c::shader::type::PHORM))
			return shader_type::PHORM;
		else if (u::iequals(type, c::shader::type::CUSTOM))
			return shader_type::CUSTOM;
		ASSERT(false);
		return shader_type::NONE;
	}
	std::string shaders::preprocess_vert(std::string const& fp)
	{
		std::ifstream in(fp);
		ASSERT(in.is_open());

		m_type = shader_type::NONE;
		m_instance_fields.clear();

		std::vector<std::pair<std::string, instance_field>> fields; // per-instance fields declared with FIELD() macros
		u32 field_offset_bytes = 0;									// per-instance field byte offset accumulator
		std::vector<std::string> lines;								// list of all lines in file
		std::string line;											// current line
		while (std::getline(in, line))
		{
			// whitespace and empty lines don't matter
			line = u::trim(line);
			if (line.empty())
				continue;

			// TYPE() macro statement
			if (line.starts_with(c::shader::macro::type))
			{
				ASSERT(m_type == shader_type::NONE);
				auto const& args = extract_args(line);
				ASSERT(args.size() == 1);
				m_type = string_to_shader_type(args[0]);
				field_offset_bytes = s_base_offset_bytes[(u64)m_type];
			}
			// FIELD() macro statement
			else if (line.starts_with(c::shader::macro::instance_field))
			{
				ASSERT(m_type != shader_type::PHORM);
				// a TYPE() statement must proceed this line
				ASSERT(field_offset_bytes > 0);
				auto const& args = extract_args(line);
				// format is either:
				//	- FIELD(type, name)
				//	- FIELD(type, name, arr_count)
				ASSERT(args.size() == 2 || args.size() == 3);

				std::string const type_name = args[0];
				GLenum const type = string_to_glsl_type(type_name);
				std::string const& name = args[1];
				// size of one element
				u32 size_bytes = glsl_type_size_bytes(type);
				// number of elements
				s32 arr_count = 1;
				// if arr_count given, use it to compute total size
				if (args.size() == 3)
				{
					arr_count = atoi(args[2].c_str());
					size_bytes *= arr_count;
				}
				// add this field
				instance_field const field = { name, type, arr_count, field_offset_bytes };
				fields.push_back({ type_name, field });
				m_instance_fields.push_back(field);
				// add total size of this field to total offset accumulator
				field_offset_bytes += size_bytes;
			}
			// regular line
			else
			{
				lines.push_back(line);
			}
		}
		ASSERT(m_type != shader_type::NONE);

		std::vector<std::string> extra_lines;

		// vbo fields (STATIC)
		if (m_type == shader_type::STATIC)
		{
			pushf(&extra_lines, "layout(location=0) in vec3 %s;", c::shader::vertex_pos.c_str());
			pushf(&extra_lines, "layout(location=1) in vec3 %s;", c::shader::vertex_norm.c_str());
			pushf(&extra_lines, "layout(location=2) in vec2 %s;", c::shader::vertex_tex.c_str());
		}
		else if (m_type == shader_type::ANIMATED)
		{
			pushf(&extra_lines, "layout(location=0) in vec3 %s;", c::shader::vertex_pos.c_str());
			pushf(&extra_lines, "layout(location=1) in vec3 %s;", c::shader::vertex_norm.c_str());
			pushf(&extra_lines, "layout(location=2) in vec2 %s;", c::shader::vertex_tex.c_str());
			ASSERT(c::shader::num_vertex_bones == 4); // can make a more sophistocated solution for these two if needed
			pushf(&extra_lines, "layout(location=3) in vec4 %s;", c::shader::vertex_bone_weight.c_str());
			pushf(&extra_lines, "layout(location=4) in uvec4 %s;", c::shader::vertex_bone_index.c_str());
		}
		else if (m_type == shader_type::PHORM)
		{
			pushf(&extra_lines, "layout(location=0) in vec3 %s;", c::shader::vertex_pos.c_str());
			pushf(&extra_lines, "layout(location=1) in vec3 %s;", c::shader::vertex_norm.c_str());
			for (u32 i = 0; i < 4; i++)
			{
				pushf(&extra_lines, "layout(location=%u) in vec4 %s;", i + 2, c::shader::vertex_uvs[i].c_str());
			}
			pushf(&extra_lines, "layout(location=6) in vec4 %s;", c::shader::vertex_texture_weights.c_str());
			pushf(&extra_lines, "layout(location=7) in vec4 %s;", c::shader::vertex_color.c_str());
		}

		// phorms do not use the instanced pipeline
		if (m_type != shader_type::PHORM)
		{
			char buf[512] = { 0 };
			// instance struct definition (CUSTOM and STATIC)
			if (m_type == shader_type::CUSTOM || m_type == shader_type::STATIC)
			{
				sprintf_s(buf, "struct %s { mat4 %s; mat3 %s; ", c::shader::instance_struct.c_str(), c::shader::instance_model_mat.c_str(), c::shader::instance_normal_mat.c_str());
			}
			else if (m_type == shader_type::ANIMATED)
			{
				sprintf_s(buf, "struct %s { mat4 %s; mat4 %s[%u]; ", c::shader::instance_struct.c_str(), c::shader::instance_model_mat.c_str(), c::shader::instance_bones.c_str(), c::shader::num_bones);
			}
			std::string struct_def = buf;
			for (auto const& pair : fields)
			{
				// array size of 1 is invalid in GLSL, so treat it as single variable instead
				if (pair.second.arr_count > 1)
					sprintf_s(buf, "%s %s[%d]; ", pair.first.c_str(),
						pair.second.name.c_str(), pair.second.arr_count);
				else
					sprintf_s(buf, "%s %s; ", pair.first.c_str(), pair.second.name.c_str());
				struct_def += buf;
			}
			struct_def += "};";
			extra_lines.push_back(struct_def);

			// instance ubo definition (ALL)
			u32 const max_structs_per_ubo =
				c::shader::ubo_size_bytes / field_offset_bytes;
			pushf(&extra_lines, "layout(std140) uniform %s { %s d_i[%u]; } %s[%u];", c::uniform::instance_block_type.c_str(), c::shader::instance_struct.c_str(), max_structs_per_ubo, c::uniform::instances.c_str(), c::shader::num_batch_ubos);

			// instance struct macro definition (ALL)
			pushf(&extra_lines, "#define d_instance %s[gl_InstanceID/%u].d_i[gl_InstanceID-%u*(gl_InstanceID/%u)]", c::uniform::instances.c_str(), max_structs_per_ubo, max_structs_per_ubo, max_structs_per_ubo);
		}

		// join all lines
		lines.insert(lines.begin() + 1, extra_lines.begin(), extra_lines.end());
		std::stringstream sstr;
		for (std::string const& line : lines)
			sstr << line << "\n";
		// printf("%u\n\n%s\n", field_offset_bytes, sstr.str().c_str());
		return sstr.str();
	}
	std::string shaders::preprocess_frag(std::string const& fp)
	{
		std::ifstream in(fp);
		ASSERT(in.is_open());

		std::vector<std::string> lines; // list of all lines in file
		std::string line;				// current line
		while (std::getline(in, line))
		{
			// whitespace and empty lines don't matter
			line = u::trim(line);
			if (line.empty())
				continue;
			if (line.starts_with(c::shader::macro::light_sum))
			{
				auto const& args = extract_args(line);
				ASSERT(args.size() == 2);
				char const* const N = args[0].c_str();
				char const* const V = args[1].c_str();
				char const* const amb = c::shader::light_ambient.c_str();
				char const* const diff = c::shader::light_diffuse.c_str();
				char const* const spec = c::shader::light_specular.c_str();
				pushf(&lines, "vec3 %s=vec3(0),%s=vec3(0),%s=vec3(0);", amb, diff, spec);
				pushf(&lines, "for(int i=0;i<int(d_lights.d_num);i++){");
				pushf(&lines, "vec3 L=normalize(d_lights.d_l[i].o2w[3].xyz);");
				pushf(&lines, "vec3 R=reflect(L,%s);", N);
				pushf(&lines, "float NdL=max(0,dot(%s,-L));", N);
				pushf(&lines, "float RdV=max(0,dot(R,%s));", V);
				pushf(&lines, "%s+=d_lights.d_l[i].ca.rgb*d_lights.d_l[i].ca.a;", amb);
				pushf(&lines, "%s+=NdL*d_lights.d_l[i].cd.rgb*d_lights.d_l[i].cd.a;", diff);
				pushf(&lines, "%s+=pow(RdV,32)*d_lights.d_l[i].cs.rgb*d_lights.d_l[i].cs.a;", spec);
				pushf(&lines, "}");
			}
			else
			{
				lines.push_back(line);
			}
		}

		std::vector<std::string> extra_lines;
		// lights
		pushf(&extra_lines, "struct %s { mat4 o2w, w2o; vec4 ca, cd, cs; float sp, rmax; float cos_tmin, cos_tmax; };", c::shader::light_struct.c_str());
		pushf(&extra_lines, "layout(std140) uniform %s { float d_num; %s d_l[%u]; } %s;", c::uniform::light_block_type.c_str(), c::shader::light_struct.c_str(), c::shader::num_lights, c::uniform::light_block_name.c_str());

		// join all lines
		lines.insert(lines.begin() + 1, extra_lines.begin(), extra_lines.end());
		std::stringstream sstr;
		for (std::string const& line : lines)
			sstr << line << "\n";
		// printf("%s\n%s\n", fp.c_str(), sstr.str().c_str());
		return sstr.str();
	}
} // namespace djinn

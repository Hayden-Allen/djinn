#include "pch.h"
#include "shaders.h"
#include "core/constants.h"

namespace djinn
{
	shaders::shaders(std::string const& vert_afp, std::string const& frag_afp) :
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
		return s_base_offset_bytes[(u64)m_type];
	}
	void shaders::init(std::string const& vert_afp, std::string const& frag_afp)
	{
		std::string const& vert_src = preprocess(vert_afp);
		std::string const& frag_src = u::read_file(frag_afp);
		super::init(vert_src, frag_src, true);
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
		if (u::iequals(type, c::shader::type::basic))
			return shader_type::BASIC;
		else if (u::iequals(type, c::shader::type::animated))
			return shader_type::ANIMATED;
		else if (u::iequals(type, c::shader::type::custom))
			return shader_type::CUSTOM;
		ASSERT(false);
		return shader_type::NONE;
	}
	std::string shaders::preprocess(std::string const& fp)
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
				// a TYPE() statement must proceed this line
				ASSERT(field_offset_bytes > 0);
				auto const& args = extract_args(line);
				// format is either:
				//	- FIELD(type, name)
				//	- FIELD(type, name, arr_count)
				ASSERT(args.size() == 2 || args.size() == 3);

				std::string type_name = args[0];
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
				instance_field field = { name, type, arr_count, field_offset_bytes };
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
		ASSERT(m_type == shader_type::BASIC); // TODO

		// instance struct definition (CUSTOM and STATIC)
		char buf[512] = { 0 };
		sprintf_s(buf, "struct %s { mat4 %s; ", c::uniform::instance_struct.c_str(), c::uniform::instance_transform.c_str());
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
		lines.insert(lines.begin() + 1, struct_def); // lines.begin() is #version

		// instance ubo definition (ALL)
		u32 const max_structs_per_ubo =
			c::shader::ubo_size_bytes / field_offset_bytes;
		sprintf_s(buf, "layout(std140) uniform %s { %s d_i[%u]; } %s[%u];", c::uniform::instance_block_type.c_str(), c::uniform::instance_struct.c_str(), max_structs_per_ubo, c::uniform::instances_uniform.c_str(), c::shader::num_ubos);
		lines.insert(lines.begin() + 2, buf); // add under struct definition

		// instance struct macro definition (ALL)
		sprintf_s(buf, "#define d_instance %s[gl_InstanceID/%u].d_i[gl_InstanceID-%u*(gl_InstanceID/%u)]", c::uniform::instances_uniform.c_str(), max_structs_per_ubo, max_structs_per_ubo, max_structs_per_ubo);
		lines.insert(lines.begin() + 3, buf); // add under ubo definition

		// join all lines
		std::stringstream sstr;
		for (std::string const& line : lines)
			sstr << line << "\n";
		// printf("%s\n", sstr.str().c_str());
		return sstr.str();
	}
} // namespace djinn

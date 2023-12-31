#pragma once
#include "pch.h"
#include "core/util.h"
#include "core/constants.h"

namespace djinn
{
	class shaders final : public mgl::shaders
	{
	public:
		struct instance_field
		{
			std::string name;
			GLenum type;
			s32 arr_count;
			u32 offset_bytes;
		};
		enum class shader_type
		{
			NONE = 0,
			BASIC,
			ANIMATED,
			CUSTOM
		};
	public:
		shaders(std::string const& vert_fp, std::string const& frag_fp);
		DCM(shaders);
	public:
		std::vector<instance_field> const& get_instance_fields() const
		{
			return m_instance_fields;
		}
	private:
		std::vector<instance_field> m_instance_fields;
	private:
		static std::string preprocess(std::string const& fp)
		{
			std::ifstream in(fp);
			if (!in.is_open())
				MGL_ASSERT(false);

			shader_type type = shader_type::NONE;
			std::vector<std::pair<std::string, instance_field>> fields;
			u32 field_offset_bytes = 64; // sizeof(mat4)
			std::vector<std::string> lines;
			std::string line;
			while (std::getline(in, line))
			{
				line = u::trim(line);
				if (line.empty())
					continue;
				if (line.starts_with("TYPE"))
				{
					auto const& args = extract_args(line);
					ASSERT(args.size() == 1);
					type = string_to_shader_type(args[0]);
				}
				else if (line.starts_with("FIELD"))
				{
					auto const& args = extract_args(line);
					ASSERT(args.size() == 2 || args.size() == 3);
					std::string type_name = args[0];
					GLenum const type = string_to_field_type(type_name);
					std::string const& name = args[1];
					u32 size_bytes = get_shader_type_size_bytes(type);
					s32 arr_count = 1;
					if (args.size() == 3)
					{
						arr_count = atoi(args[2].c_str());
						size_bytes *= arr_count;
					}
					instance_field field = { name, type, arr_count, field_offset_bytes };
					fields.push_back({ type_name, field });
					field_offset_bytes += size_bytes;
				}
				// not a preprocessor macro, add it to result
				else
				{
					lines.push_back(line);
				}
			}
			ASSERT(type == shader_type::BASIC); // TODO

			char buf[512] = { 0 };
			sprintf_s(buf, "struct %s { mat4 %s; ", c::uniform::instance_struct.c_str(), c::uniform::instance_transform.c_str());
			std::string struct_def = buf;
			for (auto const& pair : fields)
			{
				if (pair.second.arr_count > 1)
					sprintf_s(buf, "%s %s[%d]; ", pair.first.c_str(), pair.second.name.c_str(), pair.second.arr_count);
				else
					sprintf_s(buf, "%s %s; ", pair.first.c_str(), pair.second.name.c_str());
				struct_def += buf;
			}
			struct_def += "};";
			lines.insert(lines.begin() + 1, struct_def);

			u64 constexpr MAX_UBO_SIZE_BYTES = 16384;
			u64 constexpr MAX_NUM_UBOS = 12;
			u64 const max_structs_per_ubo = MAX_UBO_SIZE_BYTES / field_offset_bytes;
			sprintf_s(buf, "layout(std140,binding=0) uniform %s { %s d_i[%zu]; } %s[%zu];", c::uniform::instance_block_type.c_str(), c::uniform::instance_struct.c_str(), max_structs_per_ubo, c::uniform::instances_uniform.c_str(), MAX_NUM_UBOS);
			lines.insert(lines.begin() + 2, buf);

			sprintf_s(buf, "#define d_instance %s[gl_InstanceID/%zu].d_i[gl_InstanceID-%zu*(gl_InstanceID/%zu)]", c::uniform::instances_uniform.c_str(), max_structs_per_ubo, max_structs_per_ubo, max_structs_per_ubo);
			lines.insert(lines.begin() + 3, buf);

			std::stringstream sstr;
			for (std::string const& line : lines)
				sstr << line << "\n";
			printf("%s\n", sstr.str().c_str());
			return sstr.str();
		}
		static std::vector<std::string> extract_args(std::string const& line)
		{
			u64 const start = line.find_first_of('(') + 1;
			u64 const end = line.find_last_of(')');
			std::vector<std::string> args = u::split(line.substr(start, end - start), ",");
			for (std::string& arg : args)
				arg = u::trim(arg);
			return args;
		}
		static shader_type string_to_shader_type(std::string const& type)
		{
			if (u::iequals(type, "BASIC"))
				return shader_type::BASIC;
			else if (u::iequals(type, "ANIMATED"))
				return shader_type::ANIMATED;
			else if (u::iequals(type, "CUSTOM"))
				return shader_type::CUSTOM;
			ASSERT(false);
			return shader_type::NONE;
		}
		static GLenum string_to_field_type(std::string const& type)
		{
			if (type == "mat4")
				return GL_FLOAT_MAT4;
			else if (type == "vec4")
				return GL_FLOAT_VEC4;
			ASSERT(false);
			return 0;
		}
	};
} // namespace djinn
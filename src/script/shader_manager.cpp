#include "pch.h"
#include "shader_manager.h"
#include "core/constants.h"
#include "js.h"

namespace djinn
{
	shader_manager::shader_manager() :
		manager(c::base_dir::shader)
	{}



	id_t shader_manager::create(std::string const& vert_src, std::string const& frag_src)
	{
		shaders* const s = new shaders(vert_src, frag_src, true);
		return insert(s);
	}
	id_t shader_manager::load(std::string const& fp)
	{
		ASSERT(false);
		return 0;
	}
	void shader_manager::destroy(id_t const id)
	{
		erase(id);
	}
	void shader_manager::reload(std::string const& fp)
	{
		ASSERT(false);
	}
	void shader_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		ASSERT(false);
	}
	void shader_manager::set_uniform(JSContext* const ctx, id_t const id, std::string const& name, JSValue const& js_val)
	{
		sptr<shaders> shader = get(id);
		shader_uniform const& info = shader->get_uniform_info(name);
		switch (info.type)
		{
		case GL_FLOAT:
			shader->uniform_1f(info, js::extract_f32(ctx, js_val));
			break;
		case GL_FLOAT_VEC2:
			{
				std::vector<f32> const& v = js::extract_f32_array(ctx, js_val);
				ASSERT(v.size() == 2);
				shader->uniform_2fv(info, v.data());
			}
			break;
		case GL_FLOAT_VEC3:
			{
				std::vector<f32> const& v = js::extract_f32_array(ctx, js_val);
				ASSERT(v.size() == 3);
				shader->uniform_3fv(info, v.data());
			}
			break;
		case GL_FLOAT_VEC4:
			{
				std::vector<f32> const& v = js::extract_f32_array(ctx, js_val);
				ASSERT(v.size() == 4);
				shader->uniform_4fv(info, v.data());
			}
			break;
		default:
			ASSERT(false);
		}
	}
} // namespace djinn

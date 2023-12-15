#include "pch.h"
#include "shader_manager.h"
#include "core/constants.h"
#include "js.h"
#include "asset_service.h"

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
	id_t shader_manager::load(std::string const& vert_fp, std::string const& frag_fp)
	{
		std::string const& vert_afp = to_absolute(vert_fp);
		std::string const& frag_afp = to_absolute(frag_fp);
		shaders* const s = new shaders(vert_afp, frag_afp);
		id_t const id = insert(s);
		m_fp2ids[vert_afp].insert(id);
		m_fp2ids[frag_afp].insert(id);
		m_id2fps.insert({ id, { vert_afp, frag_afp } });
		return id;
	}
	void shader_manager::destroy(id_t const id)
	{
		// if this shader is loaded from files (made with `load` rather than `create`)
		auto const& it = m_id2fps.find(id);
		if (it != m_id2fps.end())
		{
			shader_src const& fps = it->second;
			// remove shader from its vert fp's set, and delete the set if it is now empty
			auto& vert_ids = m_fp2ids.at(fps.vert);
			vert_ids.erase(id);
			if (!vert_ids.size())
				m_fp2ids.erase(fps.vert);

			// remove shader from its frag fp's set, and delete the set if it is now empty
			auto& frag_ids = m_fp2ids.at(fps.frag);
			frag_ids.erase(id);
			if (!frag_ids.size())
				m_fp2ids.erase(fps.frag);

			// remove shader from id map
			m_id2fps.erase(id);
		}
		erase(id);
	}
	void shader_manager::reload(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		ASSERT(m_fp2ids.contains(afp));
		// reinitialize all shaders using this fp
		auto const& ids = m_fp2ids.at(afp);
		for (id_t const id : ids)
		{
			shader_src const& fps = m_id2fps.at(id);
			get(id)->init(fps.vert, fps.frag);
		}
	}
	void shader_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		std::string const& old_afp = to_absolute(old_fp);
		std::string const& new_afp = to_absolute(new_fp);
		ASSERT(m_fp2ids.contains(old_afp));
		auto const& ids = m_fp2ids.at(old_afp);
		// rename old_fp => new_fp in map
		auto entry = m_fp2ids.extract(old_afp);
		entry.key() = new_afp;
		m_fp2ids.insert(std::move(entry));

		// update fps for all ids using this shader
		for (id_t const id : ids)
		{
			shader_src& fps = m_id2fps.at(id);
			if (old_afp == fps.vert)
				fps.vert = new_afp;
			else
				fps.frag = new_afp;
		}
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
		case GL_SAMPLER_2D:
			{
				std::vector<u32> const& ids = js::extract_u32_array(ctx, js_val);
				ASSERT(ids.size() == 2); // [0] is texture id, [1] is slot
				asset_service::get_texture_manager()->bind(ids[0], ids[1]);
				shader->uniform_1i(info, ids[1]);
			}
			break;
		default:
			ASSERT(false);
		}
	}
} // namespace djinn

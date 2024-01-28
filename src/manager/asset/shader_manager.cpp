#include "pch.h"
#include "shader_manager.h"
#include "core/constants.h"
#include "script/js.h"
#include "script/service/asset_service.h"

namespace djinn
{
	shader_manager::shader_manager() :
		ref_counted_file_manager(c::base_dir::shader)
	{}



	id_t shader_manager::load(std::string const& fp)
	{
		ASSERT(false)
		return 0;
	}
	id_t shader_manager::load(std::string const& vert_fp, std::string const& frag_fp)
	{
		std::string const& vert_afp = to_absolute(vert_fp);
		std::string const& frag_afp = to_absolute(frag_fp);
		// if a shader using both vert_afp and frag_afp already exists, return its id
		id_t const existing_id = find_existing(vert_afp, frag_afp);
		if (existing_id != 0)
		{
			m_id2ref[existing_id]++;
			return existing_id;
		}

		// no such shader exists yet, create it
		shaders* const s = new shaders(vert_afp, frag_afp);
		id_t const id = insert(s);
		m_afp2ids[vert_afp].insert(id);
		m_afp2ids[frag_afp].insert(id);
		m_id2afps.insert({ id, { vert_afp, frag_afp } });
		return id;
	}
	void shader_manager::destroy(id_t const id)
	{
		if (try_erase(id))
		{
			// if this shader is loaded from files (made with `load` rather than `create`)
			auto const& it = m_id2afps.find(id);
			if (it == m_id2afps.end())
				return;
			shader_afps const& afps = it->second;
			// remove shader from its vert fp's set, and delete the set if it is now empty
			auto& vert_ids = m_afp2ids.at(afps.vert_afp);
			vert_ids.erase(id);
			if (!vert_ids.size())
				m_afp2ids.erase(afps.vert_afp);

			// remove shader from its frag fp's set, and delete the set if it is now empty
			auto& frag_ids = m_afp2ids.at(afps.frag_afp);
			frag_ids.erase(id);
			if (!frag_ids.size())
				m_afp2ids.erase(afps.frag_afp);

			// remove shader from id map
			m_id2afps.erase(id);
		}
	}
	void shader_manager::reload(std::string const& fp)
	{
		std::string const& afp = to_absolute(fp);
		if (!m_afp2ids.contains(afp))
			return;
		// reinitialize all shaders using this fp
		auto const& ids = m_afp2ids.at(afp);
		for (id_t const id : ids)
		{
			shader_afps const& afps = m_id2afps.at(id);
			get(id)->init(afps.vert_afp, afps.frag_afp);
		}
	}
	void shader_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		std::string const& old_afp = to_absolute(old_fp);
		std::string const& new_afp = to_absolute(new_fp);
		if (!m_afp2ids.contains(old_afp))
			return;
		auto const& ids = m_afp2ids.at(old_afp);
		// rename old_fp => new_fp in map
		auto entry = m_afp2ids.extract(old_afp);
		entry.key() = new_afp;
		m_afp2ids.insert(std::move(entry));

		// update fps for all ids using this shader
		for (id_t const id : ids)
		{
			shader_afps& afps = m_id2afps.at(id);
			if (old_afp == afps.vert_afp)
				afps.vert_afp = new_afp;
			else
				afps.frag_afp = new_afp;
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
				ASSERT(v.size() == 2)
				shader->uniform_2fv(info, v.data());
			}
			break;
		case GL_FLOAT_VEC3:
			{
				std::vector<f32> const& v = js::extract_f32_array(ctx, js_val);
				ASSERT(v.size() == 3)
				shader->uniform_3fv(info, v.data());
			}
			break;
		case GL_FLOAT_VEC4:
			{
				std::vector<f32> const& v = js::extract_f32_array(ctx, js_val);
				ASSERT(v.size() == 4)
				shader->uniform_4fv(info, v.data());
			}
			break;
		case GL_SAMPLER_2D:
		case GL_SAMPLER_CUBE:
			{
				u32 const slot = js::extract_u32(ctx, js_val);
				shader->uniform_1i(info, slot);
			}
			break;
		default:
			ASSERT(false)
			break;
		}
	}



	id_t shader_manager::find_existing(std::string const& vert_afp, std::string const& frag_afp)
	{
		auto const& itv = m_afp2ids.find(vert_afp);
		auto const& itf = m_afp2ids.find(frag_afp);
		// don't have at least one of the two files
		if (itv == m_afp2ids.end() || itf == m_afp2ids.end())
			return 0;
		std::unordered_set<id_t> const& ids = m_afp2ids.at(vert_afp);
		// check if any shaders that use vert_afp also use frag_afp
		for (id_t const id : ids)
		{
			if (m_id2afps.at(id).frag_afp == frag_afp)
				return id;
		}
		return 0;
	}
} // namespace djinn

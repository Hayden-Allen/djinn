#include "pch.h"
#include "light_manager.h"
#include "core/constants.h"

namespace djinn
{
	light_manager::light_manager() :
		m_ubo(s_floats_per_light * c::shader::num_lights),
		m_raw(c::shader::num_lights)
	{}



	std::vector<id_t> light_manager::load_xport(mgl::input_file* const in)
	{
		u64 const count = in->ulong();
		printf("l: %zu\n", count);
		std::vector<id_t> ids;
		ids.reserve(count);
		for (u64 i = 0; i < count; i++)
		{
			light* const l = new light(s_next_id, in);
			id_t const id = create(l);
			ids.push_back(id);
		}
		return ids;
	}
	id_t light_manager::create()
	{
		return create(new light(s_next_id));
	}
	void light_manager::destroy(id_t const id)
	{
		ASSERT(m_objects.contains(id));
		u64 const idx = m_id2idx.at(id);
		m_id2idx.erase(id);
		ASSERT(idx < m_lights.size());
		if (m_lights.size() > 1)
		{
			sptr<light> const& replacement = m_lights.back();
			m_lights.pop_back();
			m_lights[idx].bind(replacement);
			m_id2idx[replacement->get_id()] = idx;
		}
		else
		{
			m_lights.pop_back();
		}

		try_erase(id);
	}
	void light_manager::update()
	{
		f32 const num_lights = (f32)m_lights.size();
		m_ubo.update(&num_lights, 1, 0);

		for (u64 i = 0; i < m_lights.size(); i++)
		{
			// copy scene_object world transform to mgl::light
			m_lights[i]->update_transform();
			m_raw[i] = m_lights[i]->get_raw();
		}

		f32 const* const float_data = (f32*)m_raw.data();
		u32 const num_floats = (u32)m_lights.size() * s_floats_per_light;
		m_ubo.update(float_data, num_floats, s_light_offset);
		m_ubo.bind(c::uniform::light_ubo_index);
	}



	id_t light_manager::create(light* const l)
	{
		id_t const id = insert(l);
		m_lights.push_back(get(id));
		m_id2idx.insert({ id, m_lights.size() - 1 });
		return id;
	}
} // namespace djinn

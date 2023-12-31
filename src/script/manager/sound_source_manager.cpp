#include "pch.h"
#include "sound_source_manager.h"
#include "core/constants.h"
#include "script/service/sound_service.h"
#include "scene/sound_emitter.h"

namespace djinn
{
	sound_source_manager::sound_source_manager() :
		ref_counted_file_manager(c::base_dir::sound)
	{
	}
	sound_source_manager::~sound_source_manager()
	{
		unconfigure(false);
	}



	void sound_source_manager::configure()
	{
		ma_engine* engine = sound_service::get_engine();
		if (engine)
		{
			unconfigure(false);
		}
		else
		{
			unconfigure(true);
			return;
		}

		ma_resource_manager_config config = ma_resource_manager_config_init();
		config.decodedChannels = engine->pDevice->playback.channels;
		config.decodedFormat = engine->pDevice->playback.format;
		config.decodedSampleRate = engine->sampleRate;
		if (ma_resource_manager_init(&config, &m_resource_manager) != MA_SUCCESS)
		{
			unconfigure(true);
			return;
		}
		m_resource_manager_ready = true;

		reload_all();
	}
	void sound_source_manager::unconfigure(bool needs_reload_all)
	{
		if (!m_resource_manager_ready)
		{
			return;
		}

		ma_resource_manager_uninit(&m_resource_manager);
		m_resource_manager_ready = false;

		if (needs_reload_all)
		{
			reload_all();
		}
	}
	ma_resource_manager* sound_source_manager::get_resource_manager()
	{
		return m_resource_manager_ready ? &m_resource_manager : nullptr;
	}



	id_t sound_source_manager::load(std::string const& fp)
	{
		std::string const afp = to_absolute(fp);
		sound_source* source = new sound_source();
		source->load(afp);
		id_t const id = insert(source);
		m_id2afp.insert(id, afp);
		return id;
	}
	void sound_source_manager::destroy(id_t const id)
	{
		if (try_erase(id))
		{
			m_id2afp.erase_key(id);
		}
	}
	void sound_source_manager::reload(std::string const& fp)
	{
		std::string const afp = to_absolute(fp);
		auto const& ids = m_id2afp.get_key(afp);
		for (auto const& id : ids)
		{
			get(id)->load(afp);
		}
	}
	void sound_source_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		std::string const& old_afp = to_absolute(old_fp);
		if (!m_id2afp.contains_val(old_afp)) // this sound source is not loaded
			return;
		std::string const& new_afp = to_absolute(new_fp);
		m_id2afp.replace_val(old_afp, new_afp);
	}



	void sound_source_manager::reload_all()
	{
		for (auto const& pair : m_id2afp)
		{
			get(pair.first)->load(pair.second);
		}
	}
} // namespace djinn
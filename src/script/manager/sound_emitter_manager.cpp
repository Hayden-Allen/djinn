#include "pch.h"
#include "sound_emitter_manager.h"

namespace djinn
{
	sound_emitter_manager::sound_emitter_manager() :
		ref_counted_manager("")
	{}



	id_t sound_emitter_manager::create(sptr<sound_source>& source)
	{
		sound_emitter* emitter = new sound_emitter(s_next_id, source);
		id_t const id = insert(emitter);
		return id;
	}
	id_t sound_emitter_manager::load(std::string const& fp)
	{
		ASSERT(false);
		return 0;
	}
	void sound_emitter_manager::destroy(id_t const id)
	{
		try_erase(id);
	}
	void sound_emitter_manager::reload(std::string const& fp)
	{
		ASSERT(false);
	}
	void sound_emitter_manager::rename(std::string const& old_fp, std::string const& new_fp)
	{
		ASSERT(false);
	}
} // namespace djinn
#include "pch.h"
#include "sound_emitter_manager.h"

namespace djinn
{
	sound_emitter_manager::sound_emitter_manager()
	{}



	id_t sound_emitter_manager::create(sptr<sound_source>& source)
	{
		sound_emitter* emitter = new sound_emitter(s_next_id, source);
		id_t const id = insert(emitter);
		return id;
	}
	void sound_emitter_manager::destroy(id_t const id)
	{
		try_erase(id);
	}
} // namespace djinn

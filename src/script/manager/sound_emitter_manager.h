#pragma once
#include "pch.h"
#include "manager.h"
#include "scene/sound_source.h"
#include "scene/sound_emitter.h"

namespace djinn
{
	class sound_emitter_manager final : public ref_counted_manager<sound_emitter>
	{
	public:
		sound_emitter_manager();
		DCM(sound_emitter_manager);
	public:
		id_t create(sptr<sound_source>& source);
		id_t load(std::string const& fp) override;
		void destroy(id_t const id) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
	};
} // namespace djinn

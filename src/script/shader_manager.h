#pragma once
#include "pch.h"
#include "manager.h"

namespace djinn
{
	class shader_manager final : public manager<shaders>
	{
	public:
		shader_manager();
		DCM(shader_manager);
	public:
		id_t create(std::string const& vert_src, std::string const& frag_src);
		id_t load(std::string const& fp) override;
		void destroy(id_t const id) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void set_uniform(JSContext* const ctx, id_t const id, std::string const& name, JSValue const& js_val);
	};
} // namespace djinn
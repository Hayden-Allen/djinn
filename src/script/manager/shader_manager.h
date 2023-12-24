#pragma once
#include "pch.h"
#include "manager.h"

namespace djinn
{
	class shader_manager final : public ref_counted_manager<shaders>
	{
	public:
		shader_manager();
		DCM(shader_manager);
	public:
		id_t create(std::string const& vert_src, std::string const& frag_src);
		id_t load(std::string const& fp) override;
		id_t load(std::string const& vert_fp, std::string const& frag_fp);
		void destroy(id_t const id) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void set_uniform(JSContext* const ctx, id_t const id, std::string const& name, JSValue const& js_val);
		void set_uniform_mat4(id_t const id, std::string const& name, f32 const* const mat);
	private:
		struct shader_afps
		{
			std::string vert_afp, frag_afp;
		};
	private:
		std::unordered_map<std::string, std::unordered_set<id_t>> m_afp2ids;
		std::unordered_map<id_t, shader_afps> m_id2afps;
	private:
		id_t find_existing(std::string const& vert_afp, std::string const& frag_afp);
	};
} // namespace djinn

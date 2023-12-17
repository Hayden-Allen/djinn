#pragma once
#include "pch.h"
#include "manager.h"

namespace djinn
{
	class cubemap_manager final : public manager<cubemap_rgba_u8>
	{
	public:
		cubemap_manager();
		DCM(cubemap_manager);
	public:
		id_t create(u32 const width, u32 const height, texture_options const& options);
		id_t load(std::string const& fp) override;
		id_t load(std::vector<std::string> const& fps, texture_options const& options);
		void destroy(id_t const id) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void update(id_t const id, std::vector<u8> const& subpixels, texture_options const& options);
		void bind(id_t const id, u32 const slot);
	private:
		std::unordered_map<id_t, std::unordered_set<std::string>> m_id2fps;
		std::unordered_map<std::string, std::unordered_set<id_t>> m_fp2ids;
		std::unordered_map<id_t, texture_options> m_id2options;
	private:
		template<typename CONTAINER>
		std::array<u8*, 6> load(CONTAINER const& fps, s32* const out_width, s32* const out_height)
		{
			ASSERT(fps.size() == 6);
			std::array<u8*, 6> pixels;
			s32 width = -1, height = -1;
			s32 i = 0;
			for (std::string const& fp : fps)
			{
				std::string const& afp = to_absolute(fp);
				s32 new_width, new_height;
				pixels[i] = u::load_texture2d_rgba_u8_raw(afp, &new_width, &new_height);
				if (width != -1 && (new_width != width || new_height != height))
					ASSERT(false);
				width = new_width;
				height = new_height;
				i++;
			}
			*out_width = width;
			*out_height = height;
			return pixels;
		}
	};
} // namespace djinn

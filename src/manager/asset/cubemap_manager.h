#pragma once
#include "pch.h"
#include "manager/manager.h"

namespace djinn
{
	class cubemap_manager final : public ref_counted_file_manager<texture>
	{
	public:
		cubemap_manager();
		DCM(cubemap_manager);
		~cubemap_manager();
	public:
		id_t create(u32 const width, u32 const height, texture_options const& options);
		id_t load_xport(mgl::input_file* const in);
		id_t load(std::string const& fp) override;
		id_t load(std::array<std::string, 6> const& fps, texture_options const& options);
		void destroy(id_t const id) override;
		void reload(std::string const& fp) override;
		void rename(std::string const& old_fp, std::string const& new_fp) override;
		void update(id_t const id, std::array<std::vector<u8>, 6> const& subpixels, texture_options const& options);
		void update(id_t const id, std::array<std::vector<u8>, 6> const& subpixels);
		void bind(id_t const id, u32 const slot);
	private:
		std::unordered_map<id_t, std::array<std::string, 6>> m_id2afps;
		std::unordered_map<std::string, std::unordered_set<id_t>> m_afp2ids;
		std::unordered_map<id_t, texture_options> m_id2options;
		std::unordered_map<std::string, u8*> m_afp2pixels;
	private:
		std::array<u8*, 6> load(std::array<std::string, 6> const& fps, s32* const out_width, s32* const out_height)
		{
			std::array<u8*, 6> pixels = { nullptr };
			s32 width = -1, height = -1;
			for (s32 i = 0; i < 6; i++)
			{
				s32 new_width, new_height;
				std::string const& afp = to_absolute(fps[i]);
				auto const& it = m_afp2pixels.find(afp);
				// if we've already loaded this image don't load it again
				if (it != m_afp2pixels.end())
				{
					pixels[i] = it->second;
				}
				else
				{
					pixels[i] = load_one(fps[i], &new_width, &new_height);
				}

				ASSERT(width == -1 || (new_width == width && new_height == height))
				width = new_width;
				height = new_height;
			}
			*out_width = width;
			*out_height = height;
			return pixels;
		}
		u8* load_one(std::string const& fp, s32* const out_width, s32* const out_height)
		{
			std::string const& afp = to_absolute(fp);
			u8* const pixels = u::load_texture2d_rgba_u8_raw(afp, out_width, out_height);
			auto const& it = m_afp2pixels.find(afp);
			// if we've already loaded this image, get rid of the old data
			if (it != m_afp2pixels.end())
			{
				stbi_image_free(it->second);
			}
			m_afp2pixels.insert({ afp, pixels });
			return pixels;
		}
	};
} // namespace djinn

#pragma once
#include "pch.h"

namespace djinn::u
{
	extern std::string to_absolute(std::filesystem::path const& fp);
	extern std::string to_absolute(std::filesystem::path const& dir, std::string const& file);
	extern u8* load_texture2d_rgba_u8_raw(std::string const& fp, s32* const out_width, s32* const out_height);
	extern texture2d_rgba_u8* load_texture2d_rgba_u8(std::string const& fp, texture_options const& options);
	extern void sleep(u32 const ms);
	extern std::string read_file(std::string const& afp);
	extern std::vector<char> read_file_binary(std::string const& afp);
	extern std::string trim(std::string const& s);
	extern std::vector<std::string> split(std::string const& string, std::string const& delim);
	extern bool iequals(std::string const& a, std::string const& b);



	template<typename A, typename B, typename C>
	A clamp(A const x, B const min, C const max)
	{
		return (A)std::min((A)max, std::max(x, (A)min));
	}
	template<typename T>
	T sign(T const t)
	{
		constexpr T zero(0);
		if (t == zero)
			return zero;
		return (t > zero ? T(1) : T(-1));
	}
	template<typename T>
	T deg2rad(T const t)
	{
		return t * ::hats::c::PI / 180;
	}
	template<typename T>
	T rad2deg(T const t)
	{
		return t * 180 / ::hats::c::PI;
	}
	template<space FROM, space TO>
	btTransform tmat2bullet(tmat<FROM, TO> const& tmat)
	{
		// btMatrix3x3 rot(tmat.i[0], tmat.i[1], tmat.i[2], tmat.j[0], tmat.j[1], tmat.j[2], tmat.k[0], tmat.k[1], tmat.k[2]);
		btMatrix3x3 rot(tmat.i[0], tmat.j[0], tmat.k[0], tmat.i[1], tmat.j[1], tmat.k[1], tmat.i[2], tmat.j[2], tmat.k[2]);
		btVector3 trans(tmat.t[0], tmat.t[1], tmat.t[2]);
		return btTransform(rot, trans);
	}
	template<space FROM, space TO>
	tmat<FROM, TO> bullet2tmat(btTransform const& transform)
	{
		btVector3 i = transform.getBasis().getColumn(0);
		btVector3 j = transform.getBasis().getColumn(1);
		btVector3 k = transform.getBasis().getColumn(2);
		btVector3 t = transform.getOrigin();
		return tmat<FROM, TO>(
			i.x(), j.x(), k.x(), t.x(),
			i.y(), j.y(), k.y(), t.y(),
			i.z(), j.z(), k.z(), t.z());
	}
	template<space SPACE>
	btVector3 vec2bullet(vec<SPACE> const& v)
	{
		return btVector3(v.x, v.y, v.z);
	}
	template<space SPACE>
	vec<SPACE> bullet2vec(btVector3 const& v)
	{
		return vec<SPACE>(v.x(), v.y(), v.z());
	}
	template<space SPACE>
	btVector3 point2bullet(point<SPACE> const& v)
	{
		return btVector3(v.x, v.y, v.z);
	}
	template<space SPACE>
	point<SPACE> bullet2point(btVector3 const& v)
	{
		return point<SPACE>(v.x(), v.y(), v.z());
	}
	template<space SPACE>
	btVector3 direction2bullet(direction<SPACE> const& v)
	{
		return btVector3(v.x, v.y, v.z);
	}
	template<space SPACE>
	direction<SPACE> bullet2direction(btVector3 const& v)
	{
		return direction<SPACE>(v.x(), v.y(), v.z());
	}
	// https://stackoverflow.com/a/19195373
	template<typename T>
	u64 hash_combine(u64 const in, T const& v)
	{
		return in ^ (std::hash<T>{}(v) + 0x9e3779b9 + (in << 6) + (in >> 2));
	}
	template<space FROM, space TO>
	void extract_pos(tmat<FROM, TO> const& mat, f32* const x, f32* const y, f32* const z)
	{
		*x = mat.t[0];
		*y = mat.t[1];
		*z = mat.t[2];
	}
	template<space FROM, space TO>
	void extract_rot(tmat<FROM, TO> const& mat, f32* const x, f32* const y, f32* const z)
	{
		*x = asin(mat.m[2][1]);
		if (abs(mat.m[2][1]) < 0.9999999)
		{
			*y = atan2(mat.m[0][2], mat.m[2][2]);
			*z = atan2(mat.m[0][1], mat.m[1][1]);
		}
		else
		{
			*y = atan2(-mat.m[0][2], mat.m[0][0]);
			*z = 0;
		}
	}
	template<space FROM, space TO>
	void extract_scale(tmat<FROM, TO> const& mat, f32* const x, f32* const y, f32* const z)
	{
		*x = mat.get_i().length();
		*y = mat.get_j().length();
		*z = mat.get_k().length();
	}
} // namespace djinn::u

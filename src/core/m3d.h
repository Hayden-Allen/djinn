#pragma once
#include "m3d.h"

namespace djinn::m3d
{
	typedef float f32;
	extern void mat4_mul_mat4(f32* const r, f32* const a, f32* const b);
	extern void mat4_transpose(f32* const mat);
	extern void vec3_mul_quat(f32* const out, f32 const* const q);
	extern void quat_mul_quat(f32* const out, f32 const* const q);
} // namespace djinn::m3d

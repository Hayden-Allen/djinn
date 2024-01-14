#include "m3d.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#define M3D_IMPLEMENTATION
#include "model3d/m3d.h"
#ifndef NDEBUG
// #define GLAD_WRAPPER_ENABLED // FIXME: We have disabled error checking for now, so we can keep compiling on macOS until we're able to fix the error
#endif
#define GLAD_WRAPPER_IMPLEMENTATION
#include "glad/glad.h"

namespace djinn::m3d
{
	void mat4_mul_mat4(f32* const r, f32* const a, f32* const b)
	{
		r[0] = b[0] * a[0] + b[4] * a[1] + b[8] * a[2] + b[12] * a[3];
		r[1] = b[1] * a[0] + b[5] * a[1] + b[9] * a[2] + b[13] * a[3];
		r[2] = b[2] * a[0] + b[6] * a[1] + b[10] * a[2] + b[14] * a[3];
		r[3] = b[3] * a[0] + b[7] * a[1] + b[11] * a[2] + b[15] * a[3];
		r[4] = b[0] * a[4] + b[4] * a[5] + b[8] * a[6] + b[12] * a[7];
		r[5] = b[1] * a[4] + b[5] * a[5] + b[9] * a[6] + b[13] * a[7];
		r[6] = b[2] * a[4] + b[6] * a[5] + b[10] * a[6] + b[14] * a[7];
		r[7] = b[3] * a[4] + b[7] * a[5] + b[11] * a[6] + b[15] * a[7];
		r[8] = b[0] * a[8] + b[4] * a[9] + b[8] * a[10] + b[12] * a[11];
		r[9] = b[1] * a[8] + b[5] * a[9] + b[9] * a[10] + b[13] * a[11];
		r[10] = b[2] * a[8] + b[6] * a[9] + b[10] * a[10] + b[14] * a[11];
		r[11] = b[3] * a[8] + b[7] * a[9] + b[11] * a[10] + b[15] * a[11];
		r[12] = b[0] * a[12] + b[4] * a[13] + b[8] * a[14] + b[12] * a[15];
		r[13] = b[1] * a[12] + b[5] * a[13] + b[9] * a[14] + b[13] * a[15];
		r[14] = b[2] * a[12] + b[6] * a[13] + b[10] * a[14] + b[14] * a[15];
		r[15] = b[3] * a[12] + b[7] * a[13] + b[11] * a[14] + b[15] * a[15];
	}
	void mat4_transpose(f32* const mat)
	{
		for (int r = 0; r < 4; r++)
			for (int c = 0; c < r; c++)
			{
				f32 const x = mat[r * 4 + c];
				mat[r * 4 + c] = mat[c * 4 + r];
				mat[c * 4 + r] = x;
			}
	}
	void vec3_mul_quat(f32* const out, f32 const* const q)
	{
		f32 const vx = out[0], vy = out[1], vz = out[2];
		f32 const qx = q[0], qy = q[1], qz = q[2], qw = q[3];

		out[0] = vx * (qx * qx + qw * qw - qy * qy - qz * qz) + vy * (2 * qx * qy - 2 * qw * qz) + vz * (2 * qx * qz + 2 * qw * qy);
		out[1] = vx * (2 * qw * qz + 2 * qx * qy) + vy * (qw * qw - qx * qx + qy * qy - qz * qz) + vz * (-2 * qw * qx + 2 * qy * qz);
		out[2] = vx * (-2 * qw * qy + 2 * qx * qz) + vy * (2 * qw * qx + 2 * qy * qz) + vz * (qw * qw - qx * qx - qy * qy + qz * qz);
	}
	void quat_mul_quat(f32* const out, f32 const* const q)
	{
		f32 const qax = q[0], qay = q[1], qaz = q[2], qaw = q[3];
		f32 const qbx = out[0], qby = out[1], qbz = out[2], qbw = out[3];

		out[0] = qax * qbw + qaw * qbx + qay * qbz - qaz * qby;
		out[1] = qay * qbw + qaw * qby + qaz * qbx - qax * qbz;
		out[2] = qaz * qbw + qaw * qbz + qax * qby - qay * qbx;
		out[3] = qaw * qbw - qax * qbx - qay * qby - qaz * qbz;
	}
} // namespace djinn::m3d

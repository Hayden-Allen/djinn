#pragma once
#include "pch.h"
#include "core/identifiable.h"

namespace djinn
{
	class scene_object_base : public identifiable
	{
	public:
		DCM(scene_object_base);
		virtual ~scene_object_base();
	public:
		virtual void update_transform() = 0;
		virtual tmat<space::OBJECT, space::PARENT> get_transform() const = 0;
		virtual tmat<space::OBJECT, space::WORLD> get_world_transform() const = 0;
		virtual tmat<space::OBJECT, space::WORLD> get_graphics_transform() const;
		void copy_transform(sptr<scene_object_base> const& other);
	public:
		f32 const* get_pos() const;
		virtual void set_pos(f32 const x, f32 const y, f32 const z);
		virtual void set_pos_x(f32 const x);
		virtual void set_pos_y(f32 const y);
		virtual void set_pos_z(f32 const z);
		virtual void add_pos(f32 const x, f32 const y, f32 const z);
		virtual void add_pos_local(f32 const x, f32 const y, f32 const z);
	public:
		f32 const* get_rot() const;
		virtual void set_rot(f32 const x, f32 const y, f32 const z);
		virtual void set_rot_x(f32 const x);
		virtual void set_rot_y(f32 const y);
		virtual void set_rot_z(f32 const z);
		virtual void add_rot(f32 const x, f32 const y, f32 const z);
		virtual void add_rot_local(f32 const x, f32 const y, f32 const z);
	public:
		f32 const* get_scale() const;
		virtual void set_scale(f32 const x, f32 const y, f32 const z);
		virtual void set_scale_x(f32 const x);
		virtual void set_scale_y(f32 const y);
		virtual void set_scale_z(f32 const z);
		virtual void add_scale(f32 const x, f32 const y, f32 const z);
		virtual void add_scale_local(f32 const x, f32 const y, f32 const z);
	protected:
		f32 m_pos[3] = { 0 }, m_rot[3] = { 0 }, m_scale[3] = { 1, 1, 1 };
	protected:
		scene_object_base(id_t const id);
	protected:
		void extract_transform(tmat<space::OBJECT, space::PARENT> const& mat);
	};
} // namespace djinn

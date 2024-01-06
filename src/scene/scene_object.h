#pragma once
#include "pch.h"
#include "core/identifiable.h"

namespace djinn
{
	class scene_object : public identifiable
	{
	public:
		DCM(scene_object);
		virtual ~scene_object();
	public:
		virtual void update_transform();
		void set_transform(tmat<space::OBJECT, space::PARENT> const& transform);
		void set_world_transform(tmat<space::OBJECT, space::WORLD> const& transform);
		tmat<space::OBJECT, space::PARENT> const& get_transform() const;
		tmat<space::OBJECT, space::WORLD> get_world_transform() const;
	public:
		f32 const* get_pos() const;
		void set_pos(f32 const x, f32 const y, f32 const z);
		void set_pos_x(f32 const x);
		void set_pos_y(f32 const y);
		void set_pos_z(f32 const z);
		void add_pos(f32 const x, f32 const y, f32 const z);
		void add_pos_local(f32 const x, f32 const y, f32 const z);
	public:
		f32 const* get_rot() const;
		void set_rot(f32 const x, f32 const y, f32 const z);
		void set_rot_x(f32 const x);
		void set_rot_y(f32 const y);
		void set_rot_z(f32 const z);
		void add_rot(f32 const x, f32 const y, f32 const z);
		void add_rot_local(f32 const x, f32 const y, f32 const z);
	public:
		f32 const* get_scale() const;
		void set_scale(f32 const x, f32 const y, f32 const z);
		void set_scale_x(f32 const x);
		void set_scale_y(f32 const y);
		void set_scale_z(f32 const z);
		void add_scale(f32 const x, f32 const y, f32 const z);
		void add_scale_local(f32 const x, f32 const y, f32 const z);
	protected:
		tmat<space::OBJECT, space::PARENT> m_transform;
		f32 m_pos[3] = { 0 }, m_rot[3] = { 0 }, m_scale[3] = { 1, 1, 1 };
	protected:
		scene_object(id_t const id);
	};
} // namespace djinn

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
		virtual bool is_physics() const;
		virtual void set_parent(scene_object* const parent);
		void set_parent_keep_transform(scene_object* const parent);
		scene_object const* get_parent() const;
		tmat<space::PARENT, space::WORLD> get_parent_transform() const;
		tmat<space::OBJECT, space::PARENT> get_transform() const;
		tmat<space::OBJECT, space::WORLD> get_world_transform() const;
		virtual tmat<space::OBJECT, space::WORLD> get_graphics_transform() const;
		virtual void copy_transform(sptr<scene_object> const& other);
		void set_user_pointer(JSValue const& v);
		JSValue const& get_user_pointer() const;
		bool is_transform_dirty() const;
		void set_transform_clean();
	public:
		point<space::PARENT> get_pos() const;
		std::array<f32, 3> get_rot() const;
		vec<space::PARENT> get_scale() const;
		point<space::WORLD> get_pos_world() const;
		std::array<f32, 3> get_rot_world() const;
		vec<space::WORLD> get_scale_world() const;
	public:
		virtual void set_pos(point<space::PARENT> const& pos);
		virtual void set_pos_x(f32 const x);
		virtual void set_pos_y(f32 const y);
		virtual void set_pos_z(f32 const z);
		virtual void set_rot(f32 const x_deg, f32 const y_deg, f32 const z_deg);
		virtual void set_rot_x(f32 const x_deg);
		virtual void set_rot_y(f32 const y_deg);
		virtual void set_rot_z(f32 const z_deg);
		virtual void set_scale_uniform(f32 const s);
		virtual void set_scale(vec<space::PARENT> const& scale);
		virtual void set_scale_x(f32 const x);
		virtual void set_scale_y(f32 const y);
		virtual void set_scale_z(f32 const z);
	public:
		virtual void set_pos_world(point<space::WORLD> const& pos);
		virtual void set_pos_x_world(f32 const x);
		virtual void set_pos_y_world(f32 const y);
		virtual void set_pos_z_world(f32 const z);
		virtual void set_rot_world(f32 const x_deg, f32 const y_deg, f32 const z_deg);
		virtual void set_rot_x_world(f32 const x_deg);
		virtual void set_rot_y_world(f32 const y_deg);
		virtual void set_rot_z_world(f32 const z_deg);
		virtual void set_scale_world(vec<space::WORLD> const& scale);
		virtual void set_scale_x_world(f32 const x);
		virtual void set_scale_y_world(f32 const y);
		virtual void set_scale_z_world(f32 const z);
	public:
		virtual void add_pos(vec<space::PARENT> const& pos);
		virtual void add_pos_x(f32 const x);
		virtual void add_pos_y(f32 const y);
		virtual void add_pos_z(f32 const z);
		virtual void add_rot(f32 const x_deg, f32 const y_deg, f32 const z_deg);
		virtual void add_rot_x(f32 const x_deg);
		virtual void add_rot_y(f32 const y_deg);
		virtual void add_rot_z(f32 const z_deg);
		virtual void add_scale(vec<space::PARENT> const& scale);
		virtual void add_scale_x(f32 const x);
		virtual void add_scale_y(f32 const y);
		virtual void add_scale_z(f32 const z);
	public:
		virtual void add_pos_world(vec<space::WORLD> const& pos);
		virtual void add_pos_x_world(f32 const x);
		virtual void add_pos_y_world(f32 const y);
		virtual void add_pos_z_world(f32 const z);
		virtual void add_rot_world(f32 const x_deg, f32 const y_deg, f32 const z_deg);
		virtual void add_rot_x_world(f32 const x_deg);
		virtual void add_rot_y_world(f32 const y_deg);
		virtual void add_rot_z_world(f32 const z_deg);
		virtual void add_scale_world(vec<space::WORLD> const& scale);
		virtual void add_scale_x_world(f32 const x);
		virtual void add_scale_y_world(f32 const y);
		virtual void add_scale_z_world(f32 const z);
	protected:
		tmat<space::OBJECT, space::PARENT> m_transform;
		scene_object* m_parent;
		JSValue m_user;
		f32 m_rot[3] = { 0.f }; // radians
		bool m_transform_dirty = true;
	protected:
		scene_object(id_t const id);
	protected:
		void update_rot();
	};
} // namespace djinn

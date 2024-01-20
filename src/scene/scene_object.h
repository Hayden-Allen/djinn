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
		void set_parent(scene_object* const parent);
		tmat<space::OBJECT, space::PARENT> get_transform() const;
		tmat<space::PARENT, space::WORLD> get_parent_transform() const;
		tmat<space::OBJECT, space::WORLD> get_world_transform() const;
		virtual tmat<space::OBJECT, space::WORLD> get_graphics_transform() const;
		virtual void copy_transform(sptr<scene_object> const& other);
	public:
		point<space::PARENT> get_pos() const;
		std::array<f32, 3> get_rot() const;
		vec<space::PARENT> get_scale() const;
		point<space::WORLD> get_pos_world() const;
		std::array<f32, 3> get_rot_world() const;
		vec<space::WORLD> get_scale_world() const;
	public:
		void set_pos(point<space::PARENT> const& pos);
		void set_pos_x(f32 const x);
		void set_pos_y(f32 const y);
		void set_pos_z(f32 const z);
		void set_rot(f32 const x_deg, f32 const y_deg, f32 const z_deg);
		void set_rot_x(f32 const x_deg);
		void set_rot_y(f32 const y_deg);
		void set_rot_z(f32 const z_deg);
		void set_scale(vec<space::PARENT> const& scale);
		void set_scale_x(f32 const x);
		void set_scale_y(f32 const y);
		void set_scale_z(f32 const z);
	public:
		void set_pos_world(point<space::WORLD> const& pos);
		void set_pos_x_world(f32 const x);
		void set_pos_y_world(f32 const y);
		void set_pos_z_world(f32 const z);
		void set_rot_world(f32 const x_deg, f32 const y_deg, f32 const z_deg);
		void set_rot_x_world(f32 const x_deg);
		void set_rot_y_world(f32 const y_deg);
		void set_rot_z_world(f32 const z_deg);
		void set_scale_world(vec<space::WORLD> const& scale);
		void set_scale_x_world(f32 const x);
		void set_scale_y_world(f32 const y);
		void set_scale_z_world(f32 const z);
	public:
		void add_pos(vec<space::PARENT> const& pos);
		void add_pos_x(f32 const x);
		void add_pos_y(f32 const y);
		void add_pos_z(f32 const z);
		void add_rot(f32 const x_deg, f32 const y_deg, f32 const z_deg);
		void add_rot_x(f32 const x_deg);
		void add_rot_y(f32 const y_deg);
		void add_rot_z(f32 const z_deg);
		void add_scale(vec<space::PARENT> const& scale);
		void add_scale_x(f32 const x);
		void add_scale_y(f32 const y);
		void add_scale_z(f32 const z);
	public:
		void add_pos_world(vec<space::WORLD> const& pos);
		void add_pos_x_world(f32 const x);
		void add_pos_y_world(f32 const y);
		void add_pos_z_world(f32 const z);
		void add_rot_world(f32 const x_deg, f32 const y_deg, f32 const z_deg);
		void add_rot_x_world(f32 const x_deg);
		void add_rot_y_world(f32 const y_deg);
		void add_rot_z_world(f32 const z_deg);
		void add_scale_world(vec<space::WORLD> const& scale);
		void add_scale_x_world(f32 const x);
		void add_scale_y_world(f32 const y);
		void add_scale_z_world(f32 const z);
	protected:
		tmat<space::OBJECT, space::PARENT> m_transform;
		scene_object* m_parent;
		std::unordered_set<scene_object*> m_children;
		f32 m_rot[3] = { 0.f }; // radians
	protected:
		scene_object(id_t const id);
	protected:
		void update_rot();
	};
} // namespace djinn

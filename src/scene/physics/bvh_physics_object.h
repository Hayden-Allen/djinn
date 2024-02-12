#pragma once
#include "pch.h"
#include "physics_object.h"

namespace djinn
{
	class phorm;

	class bvh_physics_object final : public physics_object
	{
	public:
		bvh_physics_object(id_t const id, sptr<btDiscreteDynamicsWorld> const& world, sptr<phorm> const& phorm);
		DCM(bvh_physics_object);
		~bvh_physics_object();
	// none of these virtual methods are valid on bvh triangle mesh
	// physics_object
	public:
		void collide_and_slide(vec<space::OBJECT> const& vel, f32 const dt, vec<space::WORLD> const& threshold) override;
		void apply_impulse(vec<space::OBJECT> const& force) override;
		void set_velocity(f32 const x, f32 const y, f32 const z) override;
		void set_velocity_x(f32 const x) override;
		void set_velocity_y(f32 const y) override;
		void set_velocity_z(f32 const z) override;
		void set_velocity_world(f32 const x, f32 const y, f32 const z) override;
		void set_velocity_x_world(f32 const x) override;
		void set_velocity_y_world(f32 const y) override;
		void set_velocity_z_world(f32 const z) override;
		void set_damping(f32 const d) override;
		void set_angular_velocity(f32 const x, f32 const y, f32 const z) override;
		void set_angular_damping(f32 const d) override;
		void set_angular_factor(f32 const x, f32 const y, f32 const z) override;
		void set_max_speed(u32 const index, f32 const max) override;
		void set_gravity(vec<space::WORLD> const& force) override;
		void set_kinematic(bool const is_kinematic) override;
	// scene_object
	public:
		void set_parent(scene_object* const parent) override;
		void copy_transform(sptr<scene_object> const& other) override;
		void set_pos(point<space::PARENT> const& pos) override;
		void set_pos_x(f32 const x) override;
		void set_pos_y(f32 const y) override;
		void set_pos_z(f32 const z) override;
		void set_rot(f32 const x_deg, f32 const y_deg, f32 const z_deg) override;
		void set_rot_x(f32 const x_deg) override;
		void set_rot_y(f32 const y_deg) override;
		void set_rot_z(f32 const z_deg) override;
		void set_scale(vec<space::PARENT> const& scale) override;
		void set_scale_x(f32 const x) override;
		void set_scale_y(f32 const y) override;
		void set_scale_z(f32 const z) override;
		void set_pos_world(point<space::WORLD> const& pos) override;
		void set_pos_x_world(f32 const x) override;
		void set_pos_y_world(f32 const y) override;
		void set_pos_z_world(f32 const z) override;
		void set_rot_world(f32 const x_deg, f32 const y_deg, f32 const z_deg) override;
		void set_rot_x_world(f32 const x_deg) override;
		void set_rot_y_world(f32 const y_deg) override;
		void set_rot_z_world(f32 const z_deg) override;
		void set_scale_world(vec<space::WORLD> const& scale) override;
		void set_scale_x_world(f32 const x) override;
		void set_scale_y_world(f32 const y) override;
		void set_scale_z_world(f32 const z) override;
		void add_pos(vec<space::PARENT> const& pos) override;
		void add_pos_x(f32 const x) override;
		void add_pos_y(f32 const y) override;
		void add_pos_z(f32 const z) override;
		void add_rot(f32 const x_deg, f32 const y_deg, f32 const z_deg) override;
		void add_rot_x(f32 const x_deg) override;
		void add_rot_y(f32 const y_deg) override;
		void add_rot_z(f32 const z_deg) override;
		void add_scale(vec<space::PARENT> const& scale) override;
		void add_scale_x(f32 const x) override;
		void add_scale_y(f32 const y) override;
		void add_scale_z(f32 const z) override;
		void add_pos_world(vec<space::WORLD> const& pos) override;
		void add_pos_x_world(f32 const x) override;
		void add_pos_y_world(f32 const y) override;
		void add_pos_z_world(f32 const z) override;
		void add_rot_world(f32 const x_deg, f32 const y_deg, f32 const z_deg) override;
		void add_rot_x_world(f32 const x_deg) override;
		void add_rot_y_world(f32 const y_deg) override;
		void add_rot_z_world(f32 const z_deg) override;
		void add_scale_world(vec<space::WORLD> const& scale) override;
		void add_scale_x_world(f32 const x) override;
		void add_scale_y_world(f32 const y) override;
		void add_scale_z_world(f32 const z) override;
	private:
		optr<btTriangleIndexVertexArray> m_iva;
	};
} // namespace djinn

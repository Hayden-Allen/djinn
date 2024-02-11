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
	public:
		bool is_bvh() const override;
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
	private:
		optr<btTriangleIndexVertexArray> m_iva;
	};
} // namespace djinn

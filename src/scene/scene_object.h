// #pragma once
// #include "pch.h"
// #include "scene_object_base.h"
//
// namespace djinn
//{
//	class scene_object : public scene_object_base
//	{
//	public:
//		DCM(scene_object);
//		virtual ~scene_object();
//	public:
//		void update_transform() override;
//		tmat<space::OBJECT, space::PARENT> get_transform() const override final;
//		tmat<space::OBJECT, space::WORLD> get_world_transform() const override final;
//	protected:
//		tmat<space::OBJECT, space::PARENT> m_transform;
//	protected:
//		scene_object(id_t const id);
//	};
// } // namespace djinn

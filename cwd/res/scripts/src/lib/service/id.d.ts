declare type Optional<T> = T | undefined

// ASSET
declare interface _MeshID {
  __id: number
}
declare interface _ShaderID {
  __id: number
}
declare interface _TextureID {
  __id: number
}
declare interface _CubemapID {
  __id: number
}
declare interface _SoundID {
  __id: number
}
declare type MeshID = Optional<_MeshID>
declare type ShaderID = Optional<_ShaderID>
declare type TextureID = Optional<_TextureID>
declare type CubemapID = Optional<_CubemapID>
declare type SoundID = Optional<_SoundID>

// SCENE
declare interface _SceneID {
  __id: number
}
declare interface _MeshInstanceID extends _SceneID {}
declare interface _EntityID extends _SceneID {}
declare interface _CameraID extends _EntityID {}
declare interface _PhormID extends _SceneID {}
declare interface _PhysicsID extends _SceneID {}
declare interface _LightID extends _SceneID {}
declare interface _WaypointID extends _SceneID {}
declare type SceneID = Optional<_SceneID>
declare type MeshInstanceID = Optional<_MeshInstanceID>
declare type EntityID = Optional<_EntityID>
declare type CameraID = Optional<_CameraID>
declare type PhormID = Optional<_PhormID>
declare type PhysicsID = Optional<_PhysicsID>
declare type LightID = Optional<_LightID>
declare type WaypointID = Optional<_WaypointID>

declare interface _SoundEmitterID {
  __id: number
}
declare type SoundEmitterID = Optional<_SoundEmitterID>

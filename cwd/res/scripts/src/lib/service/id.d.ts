// ASSET
declare type _MeshID = number & { __tag: "Mesh" }
declare type _ShaderID = number & { __tag: "Shader" }
declare type _TextureID = number & { __tag: "Texture" }
declare type _CubemapID = number & { __tag: "Cubemap" }
declare type _SoundID = number & { __tag: "Sound" }

// SCENE
declare type _MeshInstanceID = number & { __tag: "MeshInstance" }
declare type _EntityID = number & { __tag: "Entity" }
declare type _CameraID = number & { __tag: "Camera" }
declare type _PhormID = number & { __tag: "Phorm" }
declare type _PhysicsID = number & { __tag: "Physics" }
declare type _LightID = number & { __tag: "Light" }
declare type _WaypointID = number & { __tag: "Waypoint" }
declare type _SoundEmitterID = number & { __tag: "SoundEmitter" }

declare type MeshID = _MeshID | undefined
declare type ShaderID = _ShaderID | undefined
declare type TextureID = _TextureID | undefined
declare type CubemapID = _CubemapID | undefined
declare type SoundID = _SoundID | undefined
declare type MeshTextureID = TextureID | CubemapID

declare type MeshInstanceID = _MeshInstanceID | undefined
declare type EntityID = _EntityID | undefined
declare type CameraID = _CameraID | undefined
declare type PhormID = _PhormID | undefined
declare type PhysicsID = _PhysicsID | undefined
declare type LightID = _LightID | undefined
declare type WaypointID = _WaypointID | undefined
declare type SoundEmitterID = _SoundEmitterID | undefined
declare type XportID = PhormID | LightID | WaypointID | undefined
declare type TaggedID =
    | EntityID
    | CameraID
    | PhormID
    | LightID
    | WaypointID
    | undefined
declare type SceneID =
    | MeshInstanceID
    | EntityID
    | CameraID
    | PhormID
    | PhysicsID
    | LightID
    | WaypointID
    | SoundEmitterID
    | undefined

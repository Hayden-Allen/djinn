// ASSET

declare type MeshID = number & { __tag: "Mesh" }
declare type ShaderID = number & { __tag: "Shader" }
declare type TextureID = number & { __tag: "Texture" }
declare type CubemapID = number & { __tag: "Cubemap" }
declare type SoundID = number & { __tag: "Sound" }

// SCENE
declare type MeshInstanceID = number & { __tag: "MeshInstance" }
declare type EntityID = number & { __tag: "Entity" }
declare type CameraID = number & { __tag: "Camera" }
declare type PhormID = number & { __tag: "Phorm" }
declare type PhysicsID = number & { __tag: "Physics" }
declare type LightID = number & { __tag: "Light" }
declare type WaypointID = number & { __tag: "Waypoint" }
declare type SceneID =
  | MeshInstanceID
  | EntityID
  | CameraID
  | PhormID
  | PhysicsID
  | LightID
  | WaypointID

declare type SoundEmitterID = number & { __tag: "SoundEmitter" }

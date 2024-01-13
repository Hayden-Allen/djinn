import "./id.d"

declare interface _Xport {
  textures: TextureID[]
  skybox: CubemapID
  phorms: PhormID[]
  lights: LightID[]
  waypoints: WaypointID[]
}

declare interface _Scene_MeshInstance {
  create(idMesh: MeshID, idShader: ShaderID): MeshInstanceID
  setUniforms(id: MeshInstanceID, map: object): void
  setAction(id: MeshInstanceID, name: string, speed?: number): void
  setVisible(id: MeshInstanceID, visible: boolean): void
  destroy(id: MeshInstanceID): void
}
declare interface _Scene_Entity {
  load(fp: string): object
  requestImGui(id: EntityID): void
  destroy(id: EntityID): void
}
declare interface _Scene_Camera {
  load(fp: string): object
  configure(
    id: CameraID,
    fovy: number,
    aspect: number,
    near: number,
    far: number
  ): void
}
declare interface _Scene_Xport {
  load(fp: string): _Xport
}
declare interface _Scene_Phorm {
  setShaders(idPhorm: PhormID, idShader: ShaderID): void
  setVisible(id: PhormID, visible: boolean): void
  destroy(id: PhormID): void
}
declare interface _Scene_Physics {
  createBox(dims: number[], origin: number[], mass: number): PhysicsID
  createSphere(radius: number, origin: number[], mass: number): PhysicsID
  createCapsuleX(
    radius: number,
    height: number,
    origin: number[],
    mass: number
  ): PhysicsID
  createCapsuleY(
    radius: number,
    height: number,
    origin: number[],
    mass: number
  ): PhysicsID
  createCapsuleZ(
    radius: number,
    height: number,
    origin: number[],
    mass: number
  ): PhysicsID
  createBVH(idPhorm: PhormID): PhysicsID
  setFriction(id: PhysicsID, f: number): void
  setLinearVelocity(id: PhysicsID, vel: number[]): void
  setAngularVelocity(id: PhysicsID, vel: number[]): void
  enableCollision(id: PhysicsID): void
  disableCollision(id: PhysicsID): void
  destroy(id: PhysicsID): void
}
declare interface _Scene_Light {
  create(): LightID
  setAmbient(id: LightID, rgba: number[]): void
  setDiffuse(id: LightID, rgba: number[]): void
  setSpecular(id: LightID, rgba: number[]): void
  destroy(id: LightID): void
}
declare interface _Scene_Waypoint {
  create(): WaypointID
  destroy(id: WaypointID): void
}
export declare interface _Scene {
  MeshInstance: _Scene_MeshInstance
  Entity: _Scene_Entity
  Camera: _Scene_Camera
  Phorm: _Scene_Phorm
  Physics: _Scene_Physics
  Light: _Scene_Light
  Xport: _Scene_Xport
  Waypoint: _Scene_Waypoint

  copyTransform(idFrom: SceneID, idTo: SceneID): void
  getPos(id: SceneID): number[]
  getPosX(id: SceneID): number
  getPosY(id: SceneID): number
  getPosZ(id: SceneID): number
  setPos(id: SceneID, xyz: number[]): void
  setPosX(id: SceneID, x: number): void
  setPosY(id: SceneID, y: number): void
  setPosZ(id: SceneID, z: number): void
  addPos(id: SceneID, xyz: number[]): void
  addPosX(id: SceneID, x: number): void
  addPosY(id: SceneID, y: number): void
  addPosZ(id: SceneID, z: number): void
  addPosLocal(id: SceneID, xyz: number[]): void
  addPosLocalX(id: SceneID, x: number): void
  addPosLocalY(id: SceneID, y: number): void
  addPosLocalZ(id: SceneID, z: number): void

  getRot(id: SceneID): number[]
  getRotX(id: SceneID): number
  getRotY(id: SceneID): number
  getRotZ(id: SceneID): number
  setRot(id: SceneID, xyz: number[]): void
  setRotX(id: SceneID, x: number): void
  setRotY(id: SceneID, y: number): void
  setRotZ(id: SceneID, z: number): void
  addRot(id: SceneID, xyz: number[]): void
  addRotX(id: SceneID, x: number): void
  addRotY(id: SceneID, y: number): void
  addRotZ(id: SceneID, z: number): void
  addRotLocal(id: SceneID, xyz: number[]): void
  addRotLocalX(id: SceneID, x: number): void
  addRotLocalY(id: SceneID, y: number): void
  addRotLocalZ(id: SceneID, z: number): void

  getScale(id: SceneID): number[]
  getScaleX(id: SceneID): number
  getScaleY(id: SceneID): number
  getScaleZ(id: SceneID): number
  setScale(id: SceneID, xyz: number[]): void
  setScaleX(id: SceneID, x: number): void
  setScaleY(id: SceneID, y: number): void
  setScaleZ(id: SceneID, z: number): void
  addScale(id: SceneID, xyz: number[]): void
  addScaleX(id: SceneID, x: number): void
  addScaleY(id: SceneID, y: number): void
  addScaleZ(id: SceneID, z: number): void
  addScaleLocal(id: SceneID, xyz: number[]): void
  addScaleLocalX(id: SceneID, x: number): void
  addScaleLocalY(id: SceneID, y: number): void
  addScaleLocalZ(id: SceneID, z: number): void
}

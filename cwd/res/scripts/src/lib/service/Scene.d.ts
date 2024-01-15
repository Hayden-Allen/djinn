import "./id.d"

declare interface _Xport {
    textures: TextureID[]
    skybox: CubemapID
    phorms: PhormID[]
    lights: LightID[]
    waypoints: WaypointID[]
}

declare interface _Scene_Waypoint {
    create(): WaypointID
    destroy(id: WaypointID): void
    destroyAll(ids: WaypointID[]): void
}
declare interface _Scene_Light {
    create(): LightID
    setAmbient(id: LightID, rgba: number[]): void
    setDiffuse(id: LightID, rgba: number[]): void
    setSpecular(id: LightID, rgba: number[]): void
    destroy(id: LightID): void
    destroyAll(ids: LightID[]): void
}
declare interface _Scene_Xport {
    load(fp: string): _Xport
}
declare interface _Scene_Phorm {
    setShaders(idPhorm: PhormID, idShader: ShaderID): void
    setAlphaShaders(idPhorm: PhormID, idShader: ShaderID): void
    setVisible(id: PhormID, visible: boolean): void
    destroy(id: PhormID): void
    destroyAll(ids: PhormID[]): void
}
declare interface _Scene_MeshInstance {
    create(idMesh: MeshID, idShader: ShaderID): MeshInstanceID
    setUniforms(id: MeshInstanceID, map: object): void
    setVisible(id: MeshInstanceID, visible: boolean): void
    setAction(id: MeshInstanceID, name: string, speed?: number): void
    getBonePos(id: MeshInstanceID, name: string): number[]
    getBoneBasisX(id: MeshInstanceID, name: string): number[]
    getBoneBasisY(id: MeshInstanceID, name: string): number[]
    getBoneBasisZ(id: MeshInstanceID, name: string): number[]
    destroy(id: MeshInstanceID): void
    destroyAll(ids: MeshInstanceID[]): void
}
declare interface _Scene_Entity {
    load(fp: string): object
    requestImGui(id: EntityID): void
    destroy(id: EntityID): void
    destroyAll(ids: EntityID[]): void
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
declare interface _Scene_Physics {
    createBox(mass: number, origin: number[], dims: number[]): PhysicsID
    createCylinder(mass: number, origin: number[], dims: number[]): PhysicsID
    createSphere(mass: number, origin: number[], radius: number): PhysicsID
    createCapsuleX(
        mass: number,
        origin: number[],
        radius: number,
        height: number
    ): PhysicsID
    createCapsuleY(
        mass: number,
        origin: number[],
        radius: number,
        height: number
    ): PhysicsID
    createCapsuleZ(
        mass: number,
        origin: number[],
        radius: number,
        height: number
    ): PhysicsID
    createBVH(idPhorm: PhormID): PhysicsID
    setFriction(id: PhysicsID, f: number): void
    setVelocity(id: PhysicsID, vel: number[]): void
    setVelocityX(id: PhysicsID, vel: number): void
    setVelocityY(id: PhysicsID, vel: number): void
    setVelocityZ(id: PhysicsID, vel: number): void
    setVelocityLocal(id: PhysicsID, vel: number[]): void
    setVelocityLocalX(id: PhysicsID, vel: number): void
    setVelocityLocalY(id: PhysicsID, vel: number): void
    setVelocityLocalZ(id: PhysicsID, vel: number): void
    setAngularVelocity(id: PhysicsID, vel: number[]): void
    setAngularFactor(id: PhysicsID, vel: number[]): void
    enableCollision(id: PhysicsID): void
    disableCollision(id: PhysicsID): void
    destroy(id: PhysicsID): void
    destroyAll(ids: PhysicsID[]): void
}
declare interface _Scene_SoundEmitter {
    create(idSource: SoundID): SoundEmitterID
    play(id: SoundEmitterID): void
    stop(id: SoundEmitterID): void
    destroy(id: SoundEmitterID): void
}
export declare interface _Scene {
    Camera: _Scene_Camera
    Entity: _Scene_Entity
    Light: _Scene_Light
    MeshInstance: _Scene_MeshInstance
    Phorm: _Scene_Phorm
    Physics: _Scene_Physics
    SoundEmitter: _Scene_SoundEmitter
    Waypoint: _Scene_Waypoint
    Xport: _Scene_Xport

    copyTransform(idFrom: SceneID, idTo: SceneID): void
    setParent(idChild: SceneID, idParent: SceneID): void
    getPos(id: SceneID): number[]
    getPosX(id: SceneID): number
    getPosY(id: SceneID): number
    getPosZ(id: SceneID): number
    getRot(id: SceneID): number[]
    getRotX(id: SceneID): number
    getRotY(id: SceneID): number
    getRotZ(id: SceneID): number
    getScale(id: SceneID): number[]
    getScaleX(id: SceneID): number
    getScaleY(id: SceneID): number
    getScaleZ(id: SceneID): number
    getPosWorld(id: SceneID): number[]
    getPosXWorld(id: SceneID): number
    getPosYWorld(id: SceneID): number
    getPosZWorld(id: SceneID): number
    getRotWorld(id: SceneID): number[]
    getRotXWorld(id: SceneID): number
    getRotYWorld(id: SceneID): number
    getRotZWorld(id: SceneID): number
    getScaleWorld(id: SceneID): number[]
    getScaleXWorld(id: SceneID): number
    getScaleYWorld(id: SceneID): number
    getScaleZWorld(id: SceneID): number
    setPos(id: SceneID, xyz: number[]): void
    setPosX(id: SceneID, x: number): void
    setPosY(id: SceneID, y: number): void
    setPosZ(id: SceneID, z: number): void
    setRot(id: SceneID, xyz: number[]): void
    setRotX(id: SceneID, x: number): void
    setRotY(id: SceneID, y: number): void
    setRotZ(id: SceneID, z: number): void
    setScale(id: SceneID, xyz: number[]): void
    setScaleX(id: SceneID, x: number): void
    setScaleY(id: SceneID, y: number): void
    setScaleZ(id: SceneID, z: number): void
    // setPosWorld(id: SceneID, xyz: number[]): void
    // setPosXWorld(id: SceneID, x: number): void
    // setPosYWorld(id: SceneID, y: number): void
    // setPosZWorld(id: SceneID, z: number): void
    // setRotWorld(id: SceneID, xyz: number[]): void
    // setRotXWorld(id: SceneID, x: number): void
    // setRotYWorld(id: SceneID, y: number): void
    // setRotZWorld(id: SceneID, z: number): void
    // setScaleWorld(id: SceneID, xyz: number[]): void
    // setScaleXWorld(id: SceneID, x: number): void
    // setScaleYWorld(id: SceneID, y: number): void
    // setScaleZWorld(id: SceneID, z: number): void
    addPos(id: SceneID, xyz: number[]): void
    addPosX(id: SceneID, x: number): void
    addPosY(id: SceneID, y: number): void
    addPosZ(id: SceneID, z: number): void
    addRot(id: SceneID, xyz: number[]): void
    addRotX(id: SceneID, x: number): void
    addRotY(id: SceneID, y: number): void
    addRotZ(id: SceneID, z: number): void
    addScale(id: SceneID, xyz: number[]): void
    addScaleX(id: SceneID, x: number): void
    addScaleY(id: SceneID, y: number): void
    addScaleZ(id: SceneID, z: number): void
    addPosWorld(id: SceneID, xyz: number[]): void
    addPosXWorld(id: SceneID, x: number): void
    addPosYWorld(id: SceneID, y: number): void
    addPosZWorld(id: SceneID, z: number): void
    addRotWorld(id: SceneID, xyz: number[]): void
    addRotXWorld(id: SceneID, x: number): void
    addRotYWorld(id: SceneID, y: number): void
    addRotZWorld(id: SceneID, z: number): void
    addScaleWorld(id: SceneID, xyz: number[]): void
    addScaleXWorld(id: SceneID, x: number): void
    addScaleYWorld(id: SceneID, y: number): void
    addScaleZWorld(id: SceneID, z: number): void
}

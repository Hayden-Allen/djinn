import "./id.d"

declare interface _Xport {
    textures: TextureID[]
    skybox: CubemapID
    phorms: PhormID[]
    lights: LightID[]
    waypoints: WaypointID[]
}

declare interface _Scene_Tag {
    add(id: TaggedID, tag: string): void
    has(id: TaggedID, tag: string): boolean
    get(id: TaggedID): string[]
    remove(id: TaggedID, tag: string): void
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
declare interface _Scene_Phorm {
    setShaders(idPhorm: PhormID, idShader: ShaderID): void
    setAlphaShaders(idPhorm: PhormID, idShader: ShaderID): void
    setVisible(id: PhormID, visible: boolean): void
    setLayer(id: PhormID, layer: number): void
    destroy(id: PhormID): void
    destroyAll(ids: PhormID[]): void
}
declare interface _Scene_Xport {
    load(fp: string): _Xport
    getName(id: XportID): string
}
declare interface _Scene_MeshInstance {
    create(idMesh: MeshID, idShader: ShaderID): MeshInstanceID
    setUniforms(
        id: MeshInstanceID,
        map: Record<string, [number | number[], number]>
    ): void
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
    load(fp: string, ...args: any[]): object
    requestImGui(id: EntityID): void
    destroy(id: EntityID): void
    destroyAll(ids: EntityID[]): void
}
declare interface _Scene_Camera {
    load(fp: string, ...args: any[]): object
    configure(
        id: CameraID,
        fovy: number,
        aspect: number,
        near: number,
        far: number
    ): void
}
declare interface _Scene_Physics {
    bind(idPhys: PhysicsID, idBound: EntityID | PhormID): void
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
    getVelocity(id: PhysicsID): number[]
    getSpeed(id: PhysicsID): number
    setVelocity(id: PhysicsID, vel: number[]): void
    setVelocityX(id: PhysicsID, vel: number): void
    setVelocityY(id: PhysicsID, vel: number): void
    setVelocityZ(id: PhysicsID, vel: number): void
    getVelocityWorld(id: PhysicsID): number[]
    getSpeedWorld(id: PhysicsID): number
    setVelocityWorld(id: PhysicsID, vel: number[]): void
    setVelocityXWorld(id: PhysicsID, vel: number): void
    setVelocityYWorld(id: PhysicsID, vel: number): void
    setVelocityZWorld(id: PhysicsID, vel: number): void
    setAngularVelocity(id: PhysicsID, vel: number[]): void
    setAngularFactor(id: PhysicsID, vel: number[]): void
    enableCollision(id: PhysicsID): void
    disableCollision(id: PhysicsID): void
    applyImpulse(id: PhysicsID, localForce: number[]): void
    setDamping(id: PhysicsID, linear: number): void
    setAngularDamping(id: PhysicsID, angular: number): void
    setMaxSpeedX(id: PhysicsID, max: number): void
    setMaxSpeedY(id: PhysicsID, max: number): void
    setMaxSpeedZ(id: PhysicsID, max: number): void
    castRay(
        fromWorld: number[],
        dirWorld: number[],
        length?: number
    ): number[][][]
    getNormalTangent(
        normWorld: number[],
        dirWorld: number[],
        idHitbox: PhysicsID
    ): number[]
    setGravity(id: PhysicsID, force: number[]): void
    setKinematic(id: PhysicsID, isKinematic: boolean): void
    setGhost(id: PhysicsID, isGhost: boolean): void
    collideNSlide(
        id: PhysicsID,
        velLocal: number[],
        dt: number,
        threshold?: Record<string, number>
    ): void
    aabbIntersects(id0: PhysicsID, id1: PhysicsID): boolean
    destroy(id: PhysicsID): void
    destroyAll(ids: PhysicsID[]): void
}
declare interface _Scene_SoundEmitter {
    create(idSource: SoundID): SoundEmitterID
    start(id: SoundEmitterID): void
    stop(id: SoundEmitterID): void
    setVolume(id: SoundEmitterID, volume: number): void
    setSpatializationEnabled(id: SoundEmitterID, enabled: boolean): void
    setLooping(id: SoundEmitterID, loop: boolean): void
    setRolloff(id: SoundEmitterID, rolloff: number): void
    setMinDistance(id: SoundEmitterID, min: number): void
    setMaxDistance(id: SoundEmitterID, max: number): void
    setAttenutation(id: SoundEmitterID, attenutation: number): void
    setFade(
        id: SoundEmitterID,
        from: number,
        to: number,
        milliseconds: number
    ): void
    destroy(id: SoundEmitterID): void
    destroyAll(ids: SoundEmitterID[]): void
}
export declare interface _Scene {
    Tag: _Scene_Tag
    Waypoint: _Scene_Waypoint
    Light: _Scene_Light
    Phorm: _Scene_Phorm
    Xport: _Scene_Xport
    MeshInstance: _Scene_MeshInstance
    Entity: _Scene_Entity
    Camera: _Scene_Camera
    Physics: _Scene_Physics
    SoundEmitter: _Scene_SoundEmitter

    copyTransform(idFrom: SceneID, idTo: SceneID): void
    setParent(idChild: SceneID, idParent: SceneID): void
    setParentKeepTransform(idChild: SceneID, idParent: SceneID): void
    unsetParent(idChild: SceneID): void
    unsetParentKeepTransform(idChild: SceneID): void
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
    setPosWorld(id: SceneID, xyz: number[]): void
    setPosXWorld(id: SceneID, x: number): void
    setPosYWorld(id: SceneID, y: number): void
    setPosZWorld(id: SceneID, z: number): void
    // setRotWorld(id: SceneID, xyz: number[]): void
    // setRotXWorld(id: SceneID, x: number): void
    // setRotYWorld(id: SceneID, y: number): void
    // setRotZWorld(id: SceneID, z: number): void
    // setScaleWorld(id: SceneID, xyz: number[]): void
    // setScaleXWorld(id: SceneID, x: number): void
    // setScaleYWorld(id: SceneID, y: number): void
    // setScaleZWorld(id: SceneID, z: number): void
    setRotParent(id: SceneID, xyz: number[]): void
    setRotXParent(id: SceneID, x: number): void
    setRotYParent(id: SceneID, y: number): void
    setRotZParent(id: SceneID, z: number): void
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

import "./lib/djinn.d"
import Entity from "./lib/Entity"
import Camera from "./lib/Camera"

const { Asset, Event, Scene, Input, ImGui, Sound } = djinn

export default class Player extends Entity {
    private camera?: Camera
    private idMainMesh: MeshID
    private idMainInstance: MeshInstanceID
    private idMainShader: ShaderID
    private idWingMesh: MeshID
    private idWingInstance: MeshInstanceID
    private idWingShader: ShaderID
    private idWingFrontTexture: TextureID
    private idWingBackTexture: TextureID
    private wingBoneNames: string[] = [
        "mixamorig:LeftArm",
        "mixamorig:LeftForeArm",
        "mixamorig:LeftHand",
        "mixamorig:LeftUpLeg",
        "mixamorig:LeftLeg",
        "mixamorig:Spine1",
    ]
    private wingUVs: number[][] = [
        [1 / 3, 1],
        [2 / 3, 1],
        [1, 1],
        [1 / 4, 1 / 3],
        [1 / 4, 0],
        [0, 2 / 3],
    ]
    private idHitbox: PhysicsID
    private isJumping: boolean = false
    private camAngleX: number = 0
    private camAngleY: number = 0
    private meshScale: number = 0.1
    private hitboxHeight: number = 1
    private hitboxRadius: number = 0.2

    // private worldPos: number[] = [-36, 103, -39] // top of tower
    // private worldPos: number[] = [-29, 30, 39] // ground
    // private worldPos: number[] = [-14, 90, 153] // gate
    private worldPos: number[] = [-28, 126, 125] // platform
    private moveDir: number[] = [0, 0, 0]
    private velY: number = 0
    private velYMin: number = -35
    private velYMax: number = 25
    private gravity: number = 75
    private canJump: boolean = false
    private lastGravity: number = 0
    private lastTime: number = 0
    private lastDelta: number[] = [0, 0, 0]

    __init(cam: Camera) {
        if (cam) {
            this.camera = cam
        }
    }
    __load() {
        ImGui.setName(this.id, "Player")
        // character
        {
            this.idMainMesh = Asset.Mesh.loadAnimated("xbot.m3d")
            this.idMainShader = Asset.Shader.load(
                "animated.vert",
                "animated.frag"
            )
            this.idMainInstance = Scene.MeshInstance.create(
                this.idMainMesh,
                this.idMainShader
            )
            Scene.MeshInstance.setAction(this.idMainInstance, "idle_Armature")
        }
        // procedural wing
        {
            this.idWingFrontTexture = Asset.Texture.load("wing_front.png")
            this.idWingBackTexture = Asset.Texture.load("wing_back.png")
            this.idWingMesh = Asset.Mesh.create(
                this.wingBoneNames.length * 2,
                [3, 3, 2, 1],
                24,
                [this.idWingFrontTexture, this.idWingBackTexture]
            )
            this.idWingShader = Asset.Shader.load("wing.vert", "wing.frag")
            Asset.Shader.setUniforms(this.idWingShader, {
                u_texture_front: 0,
                u_texture_back: 1,
            })
            this.idWingInstance = Scene.MeshInstance.create(
                this.idWingMesh,
                this.idWingShader
            )
            const idx = [
                5, 1, 0, 5, 2, 1, 3, 2, 5, 4, 2, 3, 7, 11, 6, 8, 11, 7, 8, 9,
                11, 8, 10, 9,
            ]
            Asset.Mesh.updateIndices(this.idWingMesh, idx)
        }
        // hitbox
        {
            this.idHitbox = Scene.Physics.createCapsuleY(
                1,
                this.worldPos,
                this.hitboxRadius,
                this.hitboxHeight
            )
            Scene.Physics.bind(this.idHitbox, this.id)

            Scene.Physics.setGravity(this.idHitbox, [0, 0, 0])
            Scene.Physics.setAngularFactor(this.idHitbox, [0, 0, 0])
        }
        // scene graph
        {
            Scene.setParent(this.idMainInstance, this.idHitbox)
            Scene.setRotY(this.idMainInstance, 180)
            Scene.setPosY(
                this.idMainInstance,
                -this.hitboxHeight / 2 - this.hitboxRadius
            )

            Scene.setParent(this.idWingInstance, this.idMainInstance)

            if (this.camera) {
                const idCam = this.camera!.getId()
                Scene.setParent(idCam, this.idHitbox)
                Scene.setPosZ(idCam, 2)
                Scene.setPosY(idCam, 1)
            }
        }
    }
    __unload() {
        Asset.Texture.destroyAll([
            this.idWingFrontTexture,
            this.idWingBackTexture,
        ])
        Scene.MeshInstance.destroy(this.idMainInstance)
        Asset.Mesh.destroy(this.idMainMesh)
        Asset.Shader.destroy(this.idMainShader)
        Scene.MeshInstance.destroy(this.idWingInstance)
        Asset.Mesh.destroy(this.idWingMesh)
        Asset.Shader.destroy(this.idWingShader)
        Scene.Physics.destroy(this.idHitbox)
    }
    __main(dt: number, time: number) {
        this.lastTime = time
        // wing mesh update
        {
            let frontWingVerts: number[] = []
            let backWingVerts: number[] = []
            for (var i = 0; i < this.wingBoneNames.length; i++) {
                // pos
                const pos = Scene.MeshInstance.getBonePos(
                    this.idMainInstance,
                    this.wingBoneNames[i]
                )
                if (this.isJumping) {
                    const factor = 0.01
                    pos[0] += Math.random() * factor - factor / 2
                    pos[1] += Math.random() * factor - factor / 2
                    pos[2] += Math.random() * factor - factor / 2
                }
                frontWingVerts = frontWingVerts.concat([...pos])
                backWingVerts = backWingVerts.concat(pos)
                // normal
                const basis = Scene.MeshInstance.getBoneBasisZ(
                    this.idMainInstance,
                    this.wingBoneNames[i]
                )
                basis[1] *= -1
                frontWingVerts = frontWingVerts.concat(basis)
                const minusBasis = [-basis[0], -basis[1], -basis[2]]
                backWingVerts = backWingVerts.concat(minusBasis)
                // uv
                frontWingVerts = frontWingVerts.concat(this.wingUVs[i])
                backWingVerts = backWingVerts.concat(this.wingUVs[i])
                // tex index
                frontWingVerts.push(0)
                backWingVerts.push(1)
            }
            Asset.Mesh.updateVertices(
                this.idWingMesh,
                frontWingVerts.concat(backWingVerts)
            )
        }
        // camera
        {
            this.camAngleY -= dt * 180 * Input.rightX()
            if (this.camAngleY < 0) this.camAngleY += 360
            if (this.camAngleY > 360) this.camAngleY -= 360
            Scene.setRotY(this.idHitbox, this.camAngleY)

            const newCamAngleX = this.camAngleX - dt * 90 * Input.rightY()
            if (newCamAngleX < 90 && newCamAngleX > -90)
                this.camAngleX = newCamAngleX
            Scene.setRotX(this.camera!.getId(), this.camAngleX)
        }
        // movement
        {
            const boost = Input.getKey(Input.KEY_LEFT_CONTROL) ? 5 : 1
            const x = boost * 25 * Input.leftX()
            const z = boost * 25 * Input.leftY()
            this.lastGravity = this.gravity * dt
            let newVelY = this.velY - boost * this.gravity * dt
            // if (this.canJump) {
            if (Input.getKey(Input.KEY_SPACE)) {
                this.canJump = false
                newVelY += boost * this.velYMax
                Scene.unsetParentKeepTransform(this.idHitbox)
            }
            // }
            this.velY = Math.min(
                boost * this.velYMax,
                Math.max(boost * this.velYMin, newVelY)
            )
            const dir = [x, this.velY, z]
            Scene.Physics.collideNSlide(this.idHitbox, dir, dt, { x: 1, z: 1, y: 1 })
            this.moveDir = dir

            let actionSet = false
            if (dir[0] != 0 || dir[2] != 0) {
                Scene.MeshInstance.setAction(
                    this.idMainInstance,
                    "run_Armature"
                )
                actionSet = true
            }
            if (Input.getKey(Input.KEY_SPACE)) {
                Scene.MeshInstance.setAction(this.idMainInstance, "bind")
                actionSet = true
                this.isJumping = true
            } else {
                this.isJumping = false
            }
            if (!actionSet) {
                Scene.MeshInstance.setAction(
                    this.idMainInstance,
                    "idle_Armature"
                )
            }
        }
        Scene.Entity.requestImGui(this.id)

        const idParent = Scene.getParent(this.idHitbox)
        if(idParent != 0)
        {
            // const posParent = Scene.getPosWorld(idParent)
            // const posThis = Scene.getPosWorld(this.idHitbox)
            // const newDelta = [posParent[0] - posThis[0], posParent[1] - posThis[1], posParent[2] - posThis[2]]
            // for(var i = 0; i < 3; i++)
            //     if(newDelta[i] != this.lastDelta[i])
            //     {
            //         printf(`${i} (${this.lastDelta[i]} => ${newDelta[i]}) ${dt}ms`)
            //     }
            // this.lastDelta = newDelta
            printf(idParent, Scene.getUserPointer(idParent).vel)
        }
    }
    __collide_phorm(id: PhormID, normalWorld: number[]) {
        const isTrigger = Scene.Tag.has(id, "trigger")
        if (normalWorld[1] >= 0.9 && !isTrigger) {
            if (!this.canJump) {
                // Event.dispatch("player_can_jump", "hello, world")
            }
            this.canJump = true
            this.velY = 0
            if (Scene.Tag.has(id, "switch")) {
                // Event.dispatch("player_hit_switch", Scene.Xport.getName(id))
            }
            Scene.setParentKeepTransform(this.idHitbox, id)
        }
        if (isTrigger) {
            const name = Scene.Xport.getName(id)
            const tagz = Scene.Tag.get(id)
            // printf(`Collide with: '${name}' [${tagz}]`)
        }
    }
    __no_collide() {
        // printf("No collisions")
    }
    __draw() {
        this.worldPos = Scene.getPos(this.idHitbox)
        Sound.setListenerWorldPos(this.worldPos)
        this.worldPos[1] += 1

        const dir = [
            -Math.sin((this.camAngleY * Math.PI) / 180),
            0,
            -Math.cos((this.camAngleY * Math.PI) / 180),
        ]
        Sound.setListenerWorldDir(dir)

        // update shaders AFTER CAMERA TRANSFORM IS DONE BEING MODIFIED
        Asset.Shader.setCameraUniforms(this.idMainShader, this.camera!.getId())
        // Asset.Shader.setCameraUniforms(this.idWingShader, this.camera!.getId())
    }
    __imgui() {
        ImGui.text("Pos: " + Scene.getPosWorld(this.idHitbox))
        const ivel = Scene.Physics.getVelocity(this.idHitbox).map((v) =>
            Math.round(v)
        )
        ImGui.text("Vel: " + ivel)
        ImGui.text(
            "Speed: " + Math.round(Scene.Physics.getSpeed(this.idHitbox))
        )
        ImGui.text("Cam Theta: " + Math.round(this.camAngleY))
        let moveWorld = Scene.Physics.getVelocityWorld(this.idHitbox)
        const length = Math.sqrt(
            moveWorld[0] ** 2 + moveWorld[1] ** 2 + moveWorld[2] ** 2
        )
        moveWorld = [
            moveWorld[0] / length,
            moveWorld[1] / length,
            moveWorld[2] / length,
        ]
        ImGui.text("Move World: " + moveWorld)
        ImGui.separator()

        ImGui.text("VelY: " + this.velY)
        this.velYMin = ImGui.sliderFloat("VelYMin: ", this.velYMin, -50, 0)
        this.velYMax = ImGui.sliderFloat("VelYMax: ", this.velYMax, 0, 50)
        this.gravity = ImGui.sliderFloat("Gravity: ", this.gravity, 0, 150)
    }
}

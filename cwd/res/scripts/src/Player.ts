import "./lib/djinn.d"
import Entity from "./lib/Entity"
import Camera from "./lib/Camera"

const { Asset, Scene, Input, ImGui } = djinn

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
    private meshScale: number = 0.1
    private hitboxHeight: number = 1
    private hitboxRadius: number = 0.2

    // private worldPos: number[] = [-36, 103, -39] // top of tower
    private worldPos: number[] = [-29, 30, 39] // ground
    private raycastResults: number[][][] = []
    private time: number = 0

    __init(cam: Camera) {
        if (cam) {
            this.camera = cam
        }
    }
    __load() {
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
            // this.idHitbox = Scene.Physics.createCylinder(
            //     1,
            //     this.worldPos,
            //     [0.2, 0.5, 0.2]
            // )
            this.idHitbox = Scene.Physics.createCapsuleY(
                1,
                this.worldPos,
                this.hitboxRadius,
                this.hitboxHeight
            )
            // Scene.Physics.setFriction(this.idHitbox, 5)
            // Scene.Physics.setDamping(this.idHitbox, 0.1)
            // Scene.Physics.setMaxSpeed(this.idHitbox, 5)
            Scene.Physics.setFriction(this.idHitbox, 5)
            // Scene.Physics.setDamping(this.idHitbox, 0.1)
            Scene.Physics.setMaxSpeedX(this.idHitbox, 5)
            Scene.Physics.setMaxSpeedZ(this.idHitbox, 5)
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
                Scene.setParent(this.camera!.getId(), this.idMainInstance)
                Scene.setPosZ(this.camera!.getId(), -2)
                Scene.setRotY(this.camera!.getId(), 180)
                Scene.setPosY(this.camera!.getId(), 1)
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
        this.time = time
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
        // movement
        {
            // const dx = 5 * Input.leftX()
            // const dz = 5 * Input.leftY()
            // Scene.Physics.setVelocityLocalX(this.idHitbox, dx)
            // Scene.Physics.setVelocityLocalZ(this.idHitbox, dz)
            // console.log(results.length, results[0])

            const dx = dt * 50 * Input.leftX()
            const dz = dt * 50 * Input.leftY()
            let actionSet = false
            if (dx != 0 || dz != 0) {
                this.raycastResults = Scene.Physics.castRay(
                    Scene.getPosWorld(this.idHitbox),
                    [0, -1, 0]
                )
                if (this.raycastResults.length) {
                    const tangent = Scene.Physics.getNormalTangent(
                        this.raycastResults[0][1],
                        [dx, 0, dz],
                        this.idHitbox
                    )
                    tangent[0] *= dt * 50
                    tangent[1] *= dt * 50
                    tangent[2] *= dt * 50
                    Scene.Physics.applyImpulse(this.idHitbox, [dx, 0, dz])
                    Scene.Physics.setFriction(this.idHitbox, 0)
                    // Scene.Physics.applyImpulse(this.idHitbox, tangent)
                }

                Scene.MeshInstance.setAction(
                    this.idMainInstance,
                    "run_Armature"
                )
                actionSet = true
            } else {
                Scene.Physics.setFriction(this.idHitbox, 5)
            }

            if (Input.getKey(Input.KEY_SPACE)) {
                // Scene.Physics.setVelocityY(this.idHitbox, 5)
                // Scene.Physics.applyImpulse(this.idHitbox, [0, dt * 50, 0])
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
            const ry = -2 * Input.rightX()
            Scene.Physics.setAngularVelocity(this.idHitbox, [0, ry, 0])
        }
        // camera
        {
            const newCamAngleX = this.camAngleX - dt * 90 * Input.rightY()
            if (newCamAngleX < 90 && newCamAngleX > -90)
                this.camAngleX = newCamAngleX
            Scene.setRotX(this.camera!.getId(), this.camAngleX)
        }
        Scene.Entity.requestImGui(this.id)
    }
    __draw() {
        if (this.raycastResults.length) {
            const height = this.hitboxRadius * 2 + this.hitboxHeight / 2
            const groundY = this.raycastResults[0][0][1]
            // console.log(Scene.getPosYWorld(this.idHitbox), groundY, height)
            if (
                Scene.Physics.getVelocityWorld(this.idHitbox)[1] > 0 &&
                Scene.getPosYWorld(this.idHitbox) > groundY + height
            ) {
                Scene.Physics.setVelocityYWorld(this.idHitbox, 0)
                console.log(this.time)
            }
            Scene.setPosY(this.idHitbox, groundY + 0.1)
        }

        this.worldPos = Scene.getPos(this.idHitbox)
        this.worldPos[1] += 1

        // update shaders AFTER CAMERA TRANSFORM IS DONE BEING MODIFIED
        Asset.Shader.setCameraUniforms(this.idMainShader, this.camera!.getId())
        Asset.Shader.setCameraUniforms(this.idWingShader, this.camera!.getId())
    }
    __imgui() {
        const ivel = Scene.Physics.getVelocityWorld(this.idHitbox).map((v) =>
            Math.round(v)
        )
        ImGui.text("Vel: " + ivel)
        ImGui.text(
            "Speed: " + Math.round(Scene.Physics.getSpeed(this.idHitbox))
        )
        ImGui.text(`${this.raycastResults.length}`)
        for (const result of this.raycastResults) {
            ImGui.text(`${result[0]} | ${result[1]}`)
        }
    }
}

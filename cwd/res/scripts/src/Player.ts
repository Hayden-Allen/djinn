import "./lib/djinn.d"
import Entity from "./lib/Entity"
import Camera from "./lib/Camera"

const { Asset, Scene, Input } = djinn

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

    private worldPos: number[] = [-36, 103, -39]

    bind(cam: Camera) {
        this.camera = cam
    }
    __load() {
        // character
        this.idMainMesh = Asset.Mesh.loadAnimated("xbot.m3d")
        this.idMainShader = Asset.Shader.load("animated.vert", "animated.frag")
        this.idMainInstance = Scene.MeshInstance.create(
            this.idMainMesh,
            this.idMainShader
        )
        Scene.MeshInstance.setAction(this.idMainInstance, "idle_Armature")
        // procedural wing
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
            5, 1, 0, 5, 2, 1, 3, 2, 5, 4, 2, 3, 7, 11, 6, 8, 11, 7, 8, 9, 11, 8,
            10, 9,
        ]
        Asset.Mesh.updateIndices(this.idWingMesh, idx)
        // hitbox
        this.idHitbox = Scene.Physics.createCylinder(
            1,
            this.worldPos,
            [0.2, 0.5, 0.2]
        )
        Scene.Physics.setFriction(this.idHitbox, 0)
        Scene.Physics.setAngularFactor(this.idHitbox, [0, 1, 0])
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
        // wing
        let frontWingVerts: number[] = []
        let backWingVerts: number[] = []
        for (var i = 0; i < this.wingBoneNames.length; i++) {
            // pos
            const pos = Scene.MeshInstance.getBonePos(
                this.idMainInstance,
                this.wingBoneNames[i]
            )
            if (this.isJumping) {
                const factor = 0.03
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

        // movement
        const dx = 10 * Input.leftX()
        const dz = 10 * Input.leftY()
        Scene.Physics.setVelocityLocalX(this.idHitbox, dx)
        Scene.Physics.setVelocityLocalZ(this.idHitbox, dz)
        let actionSet = false
        if (dx != 0 || dz != 0) {
            Scene.MeshInstance.setAction(this.idMainInstance, "run_Armature")
            actionSet = true
        }
        if (Input.getKey(Input.KEY_SPACE)) {
            Scene.Physics.setVelocityY(this.idHitbox, 15)
            Scene.MeshInstance.setAction(this.idMainInstance, "bind")
            actionSet = true
            this.isJumping = true
        } else {
            this.isJumping = false
        }
        if (!actionSet) {
            Scene.MeshInstance.setAction(this.idMainInstance, "idle_Armature")
        }
        const ry = 2 * Input.rightX()
        Scene.Physics.setAngularVelocity(this.idHitbox, [0, ry, 0])

        // camera
        this.camAngleX -= dt * Input.rightY()
    }
    __draw() {
        Asset.Shader.setCameraUniforms(this.idMainShader, this.camera!.getId())
        Asset.Shader.setCameraUniforms(this.idWingShader, this.camera!.getId())

        Scene.copyTransform(this.idHitbox, this.idMainInstance)

        Scene.copyTransform(this.idMainInstance, this.camera!.getId())
        Scene.addPosY(this.camera!.getId(), 1)
        // Scene.addPosZ(this.camera!.getId(), -2)
        // Scene.addRotY(this.camera!.getId(), Math.PI)
        // Scene.addRotX(this.camera!.getId(), -Math.PI / 6)

        // Scene.addPosZ(this.camera!.getId(), 2)

        Scene.addPosLocalZ(this.camera!.getId(), 2)
        Scene.addRotX(this.camera!.getId(), this.camAngleX)

        Scene.addRotY(this.idMainInstance, Math.PI)
        if (this.isJumping) {
            Scene.addRotX(this.idMainInstance, Math.PI / 2)
        }
        Scene.copyTransform(this.idMainInstance, this.idWingInstance)

        this.worldPos = Scene.getPos(this.idHitbox)
        this.worldPos[1] += 1
    }
}

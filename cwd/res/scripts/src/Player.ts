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
    private wingBoneNames: string[] = [
        "mixamorig:LeftArm",
        "mixamorig:LeftForeArm",
        "mixamorig:LeftHand",
        "mixamorig:LeftUpLeg",
        "mixamorig:LeftLeg",
        "mixamorig:Spine1",
    ]
    private idHitbox: PhysicsID

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
        this.idWingMesh = Asset.Mesh.create(
            this.wingBoneNames.length * 2,
            [3, 3],
            24,
            []
        )
        this.idWingShader = Asset.Shader.load("wing.vert", "wing.frag")
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
        this.idHitbox = Scene.Physics.createBox([0.5, 1, 5], [0, 0, 0], 1)
        Scene.Physics.setFriction(this.idHitbox, 0)
    }
    __unload() {
        Scene.MeshInstance.destroy(this.idMainInstance)
        Asset.Mesh.destroy(this.idMainMesh)
        Asset.Shader.destroy(this.idMainShader)
        Scene.MeshInstance.destroy(this.idWingInstance)
        Asset.Mesh.destroy(this.idWingMesh)
        Asset.Shader.destroy(this.idWingShader)
        Scene.Physics.destroy(this.idHitbox)
    }
    __main(dt: number, time: number) {
        let frontWingVerts: number[] = []
        let backWingVerts: number[] = []
        for (var i = 0; i < this.wingBoneNames.length; i++) {
            const pos = Scene.MeshInstance.getBonePos(
                this.idMainInstance,
                this.wingBoneNames[i]
            )
            frontWingVerts = frontWingVerts.concat(pos)
            backWingVerts = backWingVerts.concat(pos)
            const basis = Scene.MeshInstance.getBoneBasisZ(
                this.idMainInstance,
                this.wingBoneNames[i]
            )
            basis[1] *= -1
            frontWingVerts = frontWingVerts.concat(basis)
            const minusBasis = [-basis[0], -basis[1], -basis[2]]
            backWingVerts = backWingVerts.concat(minusBasis)
        }
        Asset.Mesh.updateVertices(
            this.idWingMesh,
            frontWingVerts.concat(backWingVerts)
        )

        const dx = 2 * Input.leftX()
        const dz = 2 * Input.leftY()
        Scene.Physics.setVelocityLocalX(this.idHitbox, dx)
        Scene.Physics.setVelocityLocalZ(this.idHitbox, dz)
        let actionSet = false
        if (dx != 0 || dz != 0) {
            Scene.MeshInstance.setAction(this.idMainInstance, "run_Armature")
            actionSet = true
        }
        if (Input.getKey(Input.KEY_SPACE)) {
            Scene.Physics.setVelocityY(this.idHitbox, 5)
            Scene.MeshInstance.setAction(this.idMainInstance, "bind")
            actionSet = true
        }
        if (!actionSet) {
            Scene.MeshInstance.setAction(this.idMainInstance, "idle_Armature")
        }
        // Scene.Physics.setVelocityLocal(this.idHitbox, [dx, 0, dz])

        const ry = Input.rightX()
        Scene.Physics.setAngularVelocity(this.idHitbox, [0, ry, 0])
    }
    __draw() {
        Asset.Shader.setCameraUniforms(this.idMainShader, this.camera!.getId())
        Asset.Shader.setCameraUniforms(this.idWingShader, this.camera!.getId())

        Scene.copyTransform(this.idHitbox, this.idMainInstance)

        Scene.copyTransform(this.idMainInstance, this.camera!.getId())
        Scene.addPosY(this.camera!.getId(), 1)
        Scene.addPosLocalZ(this.camera!.getId(), 2)

        Scene.addRotY(this.idMainInstance, Math.PI)
        Scene.copyTransform(this.idMainInstance, this.idWingInstance)
    }
}

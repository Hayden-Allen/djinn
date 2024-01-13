import "./lib/djinn.d"
import Entity from "./lib/Entity"
import Camera from "./lib/Camera"

const { Asset, Scene } = djinn

export default class GroundEntity extends Entity {
  private idMesh?: MeshID
  private idInstance?: MeshInstanceID
  private idPhysics?: PhysicsID
  private idShader?: ShaderID
  private idTexture?: TextureID
  private camera?: Camera

  __init() {
    this.idTexture = Asset.Texture.load("grass.png")
    this.idMesh = Asset.Mesh.create(24, [3, 3, 2], 36, [this.idTexture])
    Asset.Mesh.update(
      this.idMesh,
      [
        // front
        -1, -1, 1, 0, 0, 1, 0, 0, 1, -1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1,
        1, -1, 1, 1, 0, 0, 1, 0, 1,
        // left
        -1, -1, -1, -1, 0, 0, 0, 0, -1, -1, 1, -1, 0, 0, 1, 0, -1, 1, 1, -1, 0,
        0, 1, 1, -1, 1, -1, -1, 0, 0, 0, 1,
        // back
        1, -1, -1, 0, 0, -1, 0, 0, -1, -1, -1, 0, 0, -1, 1, 0, -1, 1, -1, 0, 0,
        -1, 1, 1, 1, 1, -1, 0, 0, -1, 0, 1,
        // right
        1, -1, 1, 1, 0, 0, 0, 0, 1, -1, -1, 1, 0, 0, 1, 0, 1, 1, -1, 1, 0, 0, 1,
        1, 1, 1, 1, 1, 0, 0, 0, 1,
        // top
        -1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, -1, 0, 1, 0, 1,
        1, -1, 1, -1, 0, 1, 0, 0, 1,
        // bottom
        -1, -1, -1, 0, -1, 0, 0, 0, 1, -1, -1, 0, -1, 0, 1, 0, 1, -1, 1, 0, -1,
        0, 1, 1, -1, -1, 1, 0, -1, 0, 0, 1,
      ],
      [
        0,
        1,
        2,
        0,
        2,
        3, // front
        4,
        5,
        6,
        4,
        6,
        7, // left
        8,
        9,
        10,
        8,
        10,
        11, // back
        12,
        13,
        14,
        12,
        14,
        15, // right
        16,
        17,
        18,
        16,
        18,
        19, // top
        20,
        21,
        22,
        20,
        22,
        23, // bottom
      ]
    )
    this.idShader = Asset.Shader.load("ground.vert", "ground.frag")
    Asset.Shader.setUniforms(this.idShader, {
      u_texture: 0,
    })
    this.idInstance = Scene.MeshInstance.create(this.idMesh, this.idShader)
    Scene.setScale(this.idInstance, [5, 1, 5])
    Scene.setPosY(this.idInstance, -2)
    this.idPhysics = Scene.Physics.createBox([5, 1, 5], [0, -2, 0], 0)
  }
  __destroy() {
    Scene.Physics.destroy(this.idPhysics!)
    Scene.MeshInstance.destroy(this.idInstance!)
    Asset.Mesh.destroy(this.idMesh!)
    Asset.Shader.destroy(this.idShader!)
    Asset.Texture.destroy(this.idTexture!)
  }
  __draw() {
    Asset.Shader.setCameraUniforms(this.idShader!, this.camera!.getId())
  }
  bind(cam: Camera) {
    this.camera = cam
  }
}

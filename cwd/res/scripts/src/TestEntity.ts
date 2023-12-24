import "./lib/globals.d"
import type { ICamera } from "./lib/Camera.d"
import Entity from "./lib/Entity"

const { Asset, Render } = djinn

export default class TestEntity extends Entity {
  private idMesh: number = -1
  private idShader: number = -1
  private idTexture: number = -1
  private meshPos: number[] = [0, 0, 0]
  private meshVel: number[] = [1, 1, 1]
  private camera: ICamera
  private color: number[] | undefined

  __init() {
    this.idMesh = Asset.Mesh.create(4, [2, 2], 6)
    Asset.Mesh.update(
      this.idMesh,
      [0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1],
      [0, 1, 2, 0, 2, 3]
    )
    this.idShader = Asset.Shader.load("test.vert", "test.frag")
    this.idTexture = Asset.Texture.load("test.bmp", {
      minFilter: GL_NEAREST,
      magFilter: GL_LINEAR,
    })
    Asset.Shader.setUniforms(this.idShader, {
      u_texture: 0,
    })
    for (var i = 0; i < 3; i++) {
      this.meshPos[i] = Math.random() * 2 - 1
      this.meshVel[i] = Math.random() * 2 - 1
    }
  }
  __destroy() {
    Asset.Mesh.destroy(this.idMesh)
    Asset.Shader.destroy(this.idShader)
    Asset.Texture.destroy(this.idTexture)
  }
  __main(dt: number) {
    for (var i = 0; i < this.meshPos.length; i++) {
      const newPos = this.meshPos[i] + dt * this.meshVel[i]
      if (newPos < -1) {
        this.meshPos[i] = -1
        this.meshVel[i] *= -1
      } else if (newPos > 1) {
        this.meshPos[i] = 1
        this.meshVel[i] *= -1
      } else {
        this.meshPos[i] = newPos
      }
    }
  }
  __draw() {
    Render.bindTexture(this.idTexture, 0)
    Asset.Shader.setCameraUniforms(this.idShader, this.camera!.getId())
    Asset.Shader.setUniforms(this.idShader, {
      u_color: this.color,
      u_pos: this.meshPos,
    })
    Render.draw(this.idMesh, this.idShader)
  }
}

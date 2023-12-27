import "./lib/globals.d"
import type { ICamera } from "./lib/Camera.d"
import Entity from "./lib/Entity"
import Color from "./lib/Color"

const { Asset, Scene } = djinn

export default class TestEntity extends Entity {
  private idInstance: number = 0
  private meshPos: number[] = [0, 0, 0]
  private meshVel: number[] = [1, 1, 1]
  private camera: ICamera
  private color: Color | undefined

  __init() {
    for (var i = 0; i < 3; i++) {
      this.meshPos[i] = Math.random() * 2 - 1
      this.meshVel[i] = Math.random() * 2 - 1
    }
  }
  __destroy() {
    Asset.Mesh.destroyInstance(this.idInstance)
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
    // Scene.setPos(this.idInstance, this.meshPos)
  }
  __draw() {
    // Render.bindTexture(this.idTexture, 0)
    // Asset.Shader.setCameraUniforms(this.idShader, this.camera!.getId())
    // Asset.Shader.setUniforms(this.idShader, {
    //   u_color: this.color!.toArray(),
    //   u_pos: this.meshPos,
    // })
    // Render.draw(this.idMesh, this.idShader)
  }
  bind(cam: ICamera, color: Color, idMesh: number) {
    this.camera = cam
    this.color = color
    this.idInstance = Scene.MeshInstance.create(idMesh)
  }
}

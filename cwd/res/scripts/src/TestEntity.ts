import "./lib/djinn.d"
import Entity from "./lib/Entity"
import Color from "./lib/Color"

const { Scene } = djinn

export default class TestEntity extends Entity {
  private idInstance: MeshInstanceID
  private meshPos: number[] = [0, 0, 0]
  private meshVel: number[] = [1, 1, 1]
  private color: Optional<Color>

  __init() {
    for (var i = 0; i < 3; i++) {
      this.meshPos[i] = (Math.random() * 2 - 1) * 10
      this.meshVel[i] = (Math.random() * 2 - 1) * 10
    }
    this.color = new Color(Math.random(), Math.random(), Math.random(), 0.5)
  }
  __destroy() {
    Scene.MeshInstance.destroy(this.idInstance)
  }
  __main(dt: number, time: number) {
    for (var i = 0; i < this.meshPos.length; i++) {
      const newPos = this.meshPos[i] + dt * this.meshVel[i]
      if (newPos < -10) {
        this.meshPos[i] = -10
        this.meshVel[i] *= -1
      } else if (newPos > 10) {
        this.meshPos[i] = 10
        this.meshVel[i] *= -1
      } else {
        this.meshPos[i] = newPos
      }
    }
    Scene.setPos(this.idInstance, this.meshPos)
  }
  __draw() {}
  bind(idMesh: MeshID, idShader: ShaderID) {
    this.idInstance = Scene.MeshInstance.create(idMesh, idShader)
    Scene.MeshInstance.setUniforms(this.idInstance, {
      color: [this.color!.toArray(), 0],
    })
  }
}

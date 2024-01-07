import "./globals.d"
import Entity from "./Entity"

const { Render, Scene, Input } = djinn

export default class Camera extends Entity {
  __load() {
    const ar = Render.getAspectRatio()
    Scene.Camera.configure(this.id, 108 / ar, ar, 0.01, 1000)
  }
  __main(dt: number) {
    const dx = dt * Input.leftX()
    const dy =
      dt * (Input.getKey(Input.KEY_SPACE) - Input.getKey(Input.KEY_SHIFT))
    const dz = dt * Input.leftY()
    const mx = dt * Input.rightX()
    const my = dt * Input.rightY()
    Scene.addPosLocal(this.id, [dx, 0, dz])
    Scene.addPosY(this.id, dy)
    Scene.addRot(this.id, [-my, -mx, 0])
  }
  __unload() {
    console.log("Destroy camera")
  }
}

import "./globals.d"
import Entity from "./Entity"
import type { _ICamera } from "./Camera.d"

const { Render, Scene, Input } = djinn

export default class Camera extends Entity implements _ICamera {
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
    let pos = Scene.getPos(this.id)
    let rot = Scene.getRot(this.id)
    pos[0] += dx
    pos[1] += dy
    pos[2] += dz
    Scene.setPos(this.id, pos)
    rot[0] -= my
    rot[1] -= mx
    Scene.setRot(this.id, rot)
  }
}

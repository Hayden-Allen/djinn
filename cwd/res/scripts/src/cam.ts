import "./lib/globals.d"
import Entity from "./lib/Entity"

const { Render, Scene, Input } = djinn

export default class Camera extends Entity {
  //   constructor(id: number) {
  //     super()
  //     this.id = id
  //     console.log("Create camera " + this.id)
  //   }
  __load() {
    const ar = Render.getAspectRatio()
    Scene.Camera.configure(this.id, 108 / ar, ar, 0.01, 1000)
  }
  __unload() {
    Scene.destroy(this.id)
  }
  __main(dt: number) {
    const dx = dt * Input.leftX()
    const dy =
      dt * (Input.getKey(Input.KEY_SPACE) - Input.getKey(Input.KEY_SHIFT))
    const dz = dt * Input.leftY()
    const mx = dt * Input.rightX()
    const my = dt * Input.rightY()
    // Scene.Camera.move(this.id, dt, dx, dy, dz, mx, my)
    Scene.Camera.move(this.id, dx, dy, dz)
    Scene.Camera.rotate(this.id, my, mx, 0)
  }
  //   static load(fp: string) {
  //     return new Camera(Scene.loadCamera(fp))
  //   }
}

import "./lib/globals.d"
import Entity from "./lib/Entity"

const { Scene, Input } = djinn

export default class Camera extends Entity {
  //   constructor(id: number) {
  //     super()
  //     this.id = id
  //     console.log("Create camera " + this.id)
  //   }
  constructor() {
    this.id = Scene.createCamera(this)
  }
  __unload() {
    Scene.destroy(this.id)
  }
  __main(dt: number) {
    const dx = Input.getKey(Input.KEY_D) - Input.getKey(Input.KEY_A)
    const dy = Input.getKey(Input.KEY_SPACE) - Input.getKey(Input.KEY_SHIFT)
    const dz = Input.getKey(Input.KEY_S) - Input.getKey(Input.KEY_W)
    const mx = Input.getKey(Input.KEY_RIGHT) - Input.getKey(Input.KEY_LEFT)
    const my = Input.getKey(Input.KEY_DOWN) - Input.getKey(Input.KEY_UP)
    Scene.moveCamera(this.id, dt, dx, dy, dz, mx, my)
  }
  //   static load(fp: string) {
  //     return new Camera(Scene.loadCamera(fp))
  //   }
}

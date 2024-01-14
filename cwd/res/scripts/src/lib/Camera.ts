import "./djinn.d"

const { Render, Scene, Input } = djinn

export default class Camera {
    protected id: CameraID = 0 as CameraID
    getId() {
        return this.id
    }
    __init() {}
    __destroy() {}
    __draw() {}
    __ui() {}
    __imgui() {}
    __unload() {}
    __load() {
        const ar = Render.getAspectRatio()
        Scene.Camera.configure(this.id, 108 / ar, ar, 0.01, 1000)
        // Scene.setPosY(this.id, 15)
    }
    __main(dt: number, time: number) {
        // dt *= 2
        // let move = 1
        // if (Input.getKey(Input.KEY_LEFT_CONTROL)) {
        //     move = 10
        // }
        // const dx = dt * move * Input.leftX()
        // const dy = dt * move * (Input.getKey(Input.KEY_SPACE) - Input.getKey(Input.KEY_SHIFT))
        // const dz = dt * move * Input.leftY()
        // const mx = dt * Input.rightX()
        // const my = dt * Input.rightY()
        // Scene.addPosLocal(this.id, [dx, 0, dz])
        // Scene.addPosY(this.id, dy)
        // Scene.addRot(this.id, [-my, -mx, 0])
    }
}

import "./djinn.d"

const { Render, Scene, Input } = djinn

export default class Camera {
    protected id: CameraID = 0 as CameraID
    getId() {
        return this.id
    }
    __init(...args: any[]) {}
    __destroy() {}
    __load() {}
    __unload() {}
    __main(dt: number, time: number) {}
    __draw() {}
    __ui() {}
    __imgui() {}
}

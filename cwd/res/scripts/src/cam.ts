import "./lib/globals.d"
import Entity from "./lib/Entity"
import Skybox from "./lib/Skybox"

const { Asset, Render, Nanovg, Scene } = djinn

export default class Camera extends Entity {
  __load() {}
  __unload() {}
  __main() {
    console.log("I am a camera")
  }
  __draw() {}
  __ui() {}
}

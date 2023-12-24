import "./lib/globals.d"
import type { ICamera } from "./lib/Camera.d"
import Entity from "./lib/Entity"
import Skybox from "./lib/Skybox"

const { Asset, Render, Nanovg, Scene, ImGui } = djinn

function genTexture(
  w: number,
  h: number,
  rmask: number,
  gmask: number,
  bmask: number
) {
  const pixels = new Array(w * h * 4)
  for (let y = 0; y < h; y++) {
    for (let x = 0; x < w; x++) {
      const off = y * (w * 4) + x * 4
      const val = Math.round(255 * (x / (w * 2) + y / (h * 2)))
      pixels[off + 0] = rmask * val
      pixels[off + 1] = gmask * val
      pixels[off + 2] = bmask * val
      pixels[off + 3] = 255
    }
  }
  return pixels
}

export default class MainEntity extends Entity {
  private skybox: Skybox | undefined
  private camera: ICamera
  private color: number[] = [0, 1, 0, 0.5]

  __init() {
    // const TW = 32,
    //   TH = 32
    // const pixels = []
    // const R_MASK = [0, 0, 0, 0, 1, 1]
    // const G_MASK = [0, 0, 1, 1, 0, 0]
    // const B_MASK = [0, 1, 0, 1, 0, 1]
    // for (let i = 0; i < 6; i++) {
    //   pixels[i] = genTexture(TW, TH, R_MASK[i], G_MASK[i], B_MASK[i])
    // }
    // this.skybox = Skybox.createGenerated(
    //   {
    //     vertexShader: "mingl/sky.vert",
    //     fragmentShader: "mingl/sky.frag",
    //   },
    //   {
    //     width: TW,
    //     height: TH,
    //     pixels,
    //     textureOptions: {
    //       minFilter: GL_NEAREST,
    //       magFilter: GL_NEAREST,
    //     },
    //   }
    // )
    // this.skybox = Skybox.loadFiles(
    //   {
    //     vertexShader: "mingl/sky.vert",
    //     fragmentShader: "mingl/sky.frag",
    //   },
    //   {
    //     front: "nz.bmp",
    //     back: "pz.bmp",
    //     left: "nx.bmp",
    //     right: "px.bmp",
    //     top: "py.bmp",
    //     bottom: "ny.bmp",
    //     textureOptions: {
    //       minFilter: GL_NEAREST,
    //       magFilter: GL_NEAREST,
    //     },
    //   }
    // )
    this.skybox = Skybox.loadDirectory(
      {
        vertexShader: "mingl/sky.vert",
        fragmentShader: "mingl/sky.frag",
      },
      {
        dir: "dir",
        textureOptions: {
          minFilter: GL_NEAREST,
          magFilter: GL_NEAREST,
        },
      }
    )
    this.camera = Scene.Camera.load("lib/Camera.js")

    for (var i = 0; i < 1000; i++) {
      let e = Scene.load("TestEntity.js")
      e.color = this.color
      e.camera = this.camera
    }
  }
  __destroy() {
    this.skybox!.destroy()
  }
  __main(dt: number) {
    Scene.requestImgui(this.id)
  }
  __draw() {
    this.skybox!.draw(this.camera!.getId())
  }
  __ui() {
    Nanovg.fillStyle(1, 1, 1)
    Nanovg.fillRect(0, 0, 300, 300)
    Nanovg.fillStyle(this.color[0], this.color[1], this.color[2])
    Nanovg.fillRect(0, 0, 50, 50)
    Nanovg.strokeStyle(1 - this.color[0], 1 - this.color[1], 1 - this.color[2])
    Nanovg.strokeRect(50, 50, 50, 50)
    Nanovg.strokeLine(50, 50, 100, 100)
    Nanovg.strokeCircle(150, 150, 50)
    Nanovg.fillCircle(200, 150, 50)
    Nanovg.setFont("sans", 24)
    Nanovg.drawText(100, 250, "TEST")
  }
  __imgui() {
    ImGui.text("I am a text box!")
    this.color = ImGui.colorPicker4("color", this.color)
  }
}

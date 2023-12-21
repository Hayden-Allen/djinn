import "./lib/globals.d"
import type { ICamera } from "./lib/Camera.d"
import Entity from "./lib/Entity"
import Skybox from "./lib/Skybox"

const { Asset, Render, Nanovg, Scene, Input } = djinn

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

export default class TestClass extends Entity {
  private idMesh: number = -1
  private idShader: number = -1
  private idTexture: number = -1
  private skybox: Skybox | undefined
  private camera: ICamera

  __init() {
    this.idMesh = Asset.Mesh.create(4, [2, 2], 6)
    Asset.Mesh.update(
      this.idMesh,
      [0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1],
      [0, 1, 2, 0, 2, 3]
    )
    this.idShader = Asset.Shader.load("test.vert", "test.frag")
    this.idTexture = Asset.Texture.load("test.bmp", {
      minFilter: GL_NEAREST,
      magFilter: GL_LINEAR,
    })
    Asset.Shader.setUniforms(this.idShader, {
      u_texture: 0,
    })
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
  }
  __destroy() {
    this.skybox!.destroy()
    Asset.Mesh.destroy(this.idMesh)
    Asset.Shader.destroy(this.idShader)
    Asset.Texture.destroy(this.idTexture)
  }
  __main(dt: number) {}
  __draw() {
    this.skybox!.draw(this.camera!.getId())
    Render.bindTexture(this.idTexture, 0)
    Asset.Shader.setCameraUniforms(this.idShader, this.camera!.getId())
    Render.draw(this.idMesh, this.idShader)
  }
  __ui() {
    Nanovg.fillStyle(1, 1, 1)
    Nanovg.fillRect(0, 0, 300, 300)
    Nanovg.fillStyle(0, 1, 0, 1)
    Nanovg.fillRect(0, 0, 50, 50)
    Nanovg.strokeStyle(1, 0, 1)
    Nanovg.strokeRect(50, 50, 50, 50)
    Nanovg.strokeLine(50, 50, 100, 100)
    Nanovg.strokeCircle(150, 150, 50)
    Nanovg.fillCircle(200, 150, 50)
    Nanovg.fillStyle(0, 0, 0)
    Nanovg.setFont("sans", 24)
    Nanovg.drawText(100, 250, "TEST")
  }
}

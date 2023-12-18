import "./lib/globals.d"
import Entity from "./lib/Entity"
import Skybox from "./lib/Skybox"

const { Asset, Render, Nanovg } = djinn
export default class TestClass extends Entity {
  private idMesh: number = -1
  private idShader: number = -1
  private idTexture: number = -1
  private skybox: Skybox | undefined

  __load() {
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

    this.skybox = new Skybox({
      vertexShader: "mingl/sky.vert",
      fragmentShader: "mingl/sky.frag",
      textures: {
        front: "nz.bmp",
        back: "pz.bmp",
        left: "nx.bmp",
        right: "px.bmp",
        top: "py.bmp",
        bottom: "ny.bmp",
      },
      textureOptions: {
        minFilter: GL_NEAREST,
        magFilter: GL_NEAREST,
      },
    })
  }
  __unload() {
    this.skybox!.unload()
    Asset.Mesh.destroy(this.idMesh)
    Asset.Shader.destroy(this.idShader)
    Asset.Texture.destroy(this.idTexture)
  }
  __main() {}
  __draw() {
    this.skybox!.draw()

    Render.bindTexture(this.idTexture, 0)
    Render.draw(this.idMesh, this.idShader)

    Nanovg.fillStyle(1, 1, 1)
    Nanovg.fillRect(0, 0, 300, 300)
    Nanovg.fillStyle(0, 1, 0, 1)
    Nanovg.fillRect(0, 0, 50, 50)
    Nanovg.strokeStyle(1, 0, 1)
    Nanovg.strokeRect(50, 50, 50, 50)
    Nanovg.strokeLine(50, 50, 100, 100)
    Nanovg.strokeCircle(150, 150, 50)
    Nanovg.fillCircle(200, 150, 50)
  }
}

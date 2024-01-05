import "./lib/globals.d"
import type { ICamera } from "./lib/Camera.d"
import Entity from "./lib/Entity"
import Skybox from "./lib/Skybox"
import TestEntity from "./TestEntity"
import Color from "./lib/Color"

const { Asset, Render, Nanovg, Scene, ImGui, Sound } = djinn

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
  private color: Color = new Color(0, 0, 0, 1)
  private entities: TestEntity[] = []
  private idShader: number = 0
  private idTexture: number = 0
  private idMesh: number = 0
  private idSoundSource: number = 0
  private idSoundEmitter: number = 0
  private needsPlayAudio: boolean = true

  private idStaticMesh: number = 0
  private idStaticInstance: number = 0
  private idStaticShader: number = 0

  private idAnimatedMesh: number = 0
  private idAnimatedInstances: number[] = []
  private idAnimatedShader: number = 0

  __init() {
    this.skybox = Skybox.loadDirectory(
      {
        vertexShader: "sky.vert",
        fragmentShader: "sky.frag",
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
    this.idShader = Asset.Shader.load("test.vert", "test.frag")
    this.idTexture = Asset.Texture.load("test.bmp", {
      minFilter: GL_NEAREST,
      magFilter: GL_LINEAR,
    })
    Asset.Shader.setUniforms(this.idShader, {
      u_texture: 0,
    })
    this.idMesh = Asset.Mesh.create(4, [2, 2], 6, [this.idTexture])
    Asset.Mesh.update(
      this.idMesh,
      [0, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1],
      [0, 1, 2, 0, 2, 3]
    )
    this.idSoundSource = Asset.Sound.load("test.mp3")
    this.idSoundEmitter = Sound.Emitter.create(this.idSoundSource)
    this.needsPlayAudio = true

    this.idStaticMesh = Asset.Mesh.loadStatic("suzanne.m3d")
    this.idStaticShader = Asset.Shader.load("static.vert", "static.frag")
    this.idStaticInstance = Scene.MeshInstance.create(
      this.idStaticMesh,
      this.idStaticShader
    )
    Scene.setPosX(this.idStaticInstance, -2)
    Scene.setPosZ(this.idStaticInstance, -3)

    this.idAnimatedMesh = Asset.Mesh.loadAnimated("samba-dancing.m3d")
    this.idAnimatedShader = Asset.Shader.load("animated.vert", "animated.frag")
    for (var i = 0; i < 10; i++) {
      this.idAnimatedInstances.push(
        Scene.MeshInstance.create(this.idAnimatedMesh, this.idAnimatedShader)
      )
      Scene.setPosX(this.idAnimatedInstances[i], -5 + i)
      Scene.setPosZ(this.idAnimatedInstances[i], -3)
    }
    // Scene.setRotZ(this.idAnimatedInstance, Math.PI / 2)
    // Scene.setRotY(this.idAnimatedInstance, Math.PI / 2)
  }
  __destroy() {
    Scene.MeshInstance.destroy(this.idStaticInstance)
    Asset.Mesh.destroy(this.idStaticMesh)
    Asset.Shader.destroy(this.idStaticShader)
    for (var i = 0; i < this.idAnimatedInstances.length; i++)
      Scene.MeshInstance.destroy(this.idAnimatedInstances[i])
    Asset.Mesh.destroy(this.idAnimatedMesh)
    Asset.Shader.destroy(this.idAnimatedShader)

    this.skybox!.destroy()

    Sound.Emitter.stop(this.idSoundEmitter)
    Sound.Emitter.destroy(this.idSoundEmitter)
    Asset.Sound.destroy(this.idSoundSource)
    Asset.Mesh.destroy(this.idMesh)
    Asset.Shader.destroy(this.idShader)
    Asset.Texture.destroy(this.idTexture)
  }
  __load() {
    this.color.set(0, 1, 1, 0.5)
    for (var i = 0; i < 1000; i++) {
      let e = Scene.load("TestEntity.js")
      e.bind(this.camera, this.color, this.idMesh, this.idShader)
      this.entities.push(e)
    }
  }
  __unload() {
    for (var i = 0; i < this.entities.length; i++)
      Scene.destroy(this.entities[i].getId())
    this.entities = []
  }
  __main(dt: number) {
    if (this.needsPlayAudio) {
      // Sound.Emitter.play(this.idSoundEmitter)
      this.needsPlayAudio = false
    }
    Scene.addRotY(this.idStaticInstance, dt)
    // Scene.addRotY(this.idAnimatedInstance, -dt)
    Scene.Entity.requestImgui(this.id)
  }
  __draw() {
    this.skybox!.draw(this.camera!.getId())
    // Asset.Shader.setUniforms(this.idShader, {
    //   u_color: this.color!.toArray(),
    // })
    Asset.Shader.setCameraUniforms(this.idShader, this.camera!.getId())
    Asset.Shader.setCameraUniforms(this.idStaticShader, this.camera!.getId())
    Asset.Shader.setCameraUniforms(this.idAnimatedShader, this.camera!.getId())
  }
  __ui() {
    Nanovg.fillStyle(1, 1, 1)
    Nanovg.fillRect(0, 0, 300, 300)
    Nanovg.fillStyle(this.color.r, this.color.g, this.color.b)
    Nanovg.fillRect(0, 0, 50, 50)
    Nanovg.strokeStyle(1 - this.color.r, 1 - this.color.g, 1 - this.color.b)
    Nanovg.strokeRect(50, 50, 50, 50)
    Nanovg.strokeLine(50, 50, 100, 100)
    Nanovg.strokeCircle(150, 150, 50)
    Nanovg.fillCircle(200, 150, 50)
    Nanovg.setFont("sans", 24)
    Nanovg.drawText(100, 250, "TEST")
  }
  __imgui() {
    ImGui.text("I am a text box!")
    this.color.fromArray(ImGui.colorPicker4("color", this.color.toArray()))
  }
}

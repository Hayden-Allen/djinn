import "./lib/globals.d"
import Camera from "./lib/Camera"
import Entity from "./lib/Entity"
import Skybox from "./lib/Skybox"
import Color from "./lib/Color"
import TestEntity from "./TestEntity"
import GroundEntity from "./GroundEntity"

const { Asset, Render, Nanovg, Scene, ImGui, Sound, Input } = djinn

export default class MainEntity extends Entity {
  private skybox: Optional<Skybox>
  private camera: Optional<Camera>
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

  private frame: number = 0
  private nextAnimated: number = 0

  private idPhysics: number = 0
  private ground: Optional<GroundEntity>

  private idPhormShader: number = 0
  private idPhormShader2: number = 0
  private idPhorms: number[] = []

  private idLight: number = 0

  __init() {
    this.skybox = Skybox.loadDirectory(
      {
        vertexShader: "sky.vert",
        fragmentShader: "sky.frag",
      },
      {
        dir: "dir",
        textureOptions: {
          minFilter: Render.GL_NEAREST,
          magFilter: Render.GL_NEAREST,
        },
      }
    )
    this.camera = Scene.Camera.load("lib/Camera.js") as Camera
    this.idShader = Asset.Shader.load("custom.vert", "custom.frag")
    this.idTexture = Asset.Texture.load("test.bmp", {
      minFilter: Render.GL_NEAREST,
      magFilter: Render.GL_LINEAR,
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

    this.idAnimatedMesh = Asset.Mesh.loadAnimated("xbot.m3d")
    this.idAnimatedShader = Asset.Shader.load("animated.vert", "animated.frag")
    for (var i = 0; i < 10; i++) {
      this.idAnimatedInstances.push(
        Scene.MeshInstance.create(this.idAnimatedMesh, this.idAnimatedShader)
      )
      Scene.setPosX(this.idAnimatedInstances[i], -5 + i)
      Scene.setPosZ(this.idAnimatedInstances[i], -3)
    }

    this.idPhysics = Scene.Physics.create([1, 1, 1], [-2, 5, -3], 1)
    Scene.Physics.setFriction(this.idPhysics, 0)
    Scene.Physics.setAngularVelocity(this.idPhysics, [0, 1, 0])

    this.ground = Scene.Entity.load("GroundEntity.js") as GroundEntity
    this.ground?.bind(this.camera!)

    this.idPhormShader = Asset.Shader.load("phorm.vert", "phorm.frag")
    this.idPhormShader2 = Asset.Shader.load("phorm2.vert", "phorm.frag")
    this.idPhorms = Scene.Phorm.load("city.xport")
    for (var i = 0; i < this.idPhorms.length - 1; i++)
      Scene.Phorm.setShaders(this.idPhorms[i], this.idPhormShader)
    Scene.Phorm.setShaders(
      this.idPhorms[this.idPhorms.length - 1],
      this.idPhormShader2
    )

    this.idLight = Scene.Light.create()
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

    Scene.Physics.destroy(this.idPhysics)

    for (const id of this.idPhorms) {
      Scene.Phorm.destroy(id)
    }
    Asset.Shader.destroy(this.idPhormShader)
    Asset.Shader.destroy(this.idPhormShader2)

    Scene.Light.destroy(this.idLight)
  }
  __load() {
    this.color.set(0, 1, 1, 0.5)
    for (var i = 0; i < 100; i++) {
      let e = Scene.Entity.load("TestEntity.js") as TestEntity
      e.bind(this.camera!, this.color, this.idMesh, this.idShader)
      this.entities.push(e)
    }
  }
  __unload() {
    for (var i = 0; i < this.entities.length; i++)
      Scene.Entity.destroy(this.entities[i].getId())
    this.entities = []
  }
  __main(dt: number, time: number) {
    this.frame++
    if (
      this.nextAnimated < this.idAnimatedInstances.length &&
      this.frame >= this.nextAnimated * 100
    ) {
      Scene.MeshInstance.setAction(
        this.idAnimatedInstances[this.nextAnimated],
        "run_Armature",
        1 + this.nextAnimated / 10
      )
      this.nextAnimated++
    }

    if (this.needsPlayAudio) {
      // Sound.Emitter.play(this.idSoundEmitter)
      this.needsPlayAudio = false
    }
    Scene.Entity.requestImGui(this.id)

    if (Input.getKey(Input.KEY_SPACE)) {
      Scene.Physics.setLinearVelocity(this.idPhysics, [0, 5, 0])
      Scene.MeshInstance.setVisible(this.idStaticInstance, false)
    } else {
      Scene.MeshInstance.setVisible(this.idStaticInstance, true)
    }

    for (const id of this.idPhorms) {
      Scene.addRotY(id, dt)
      const s = Math.abs(Math.sin(time))
      Scene.setScale(id, [s, s, s])
    }
  }
  __draw() {
    this.skybox!.draw(this.camera!.getId())
    // Asset.Shader.setUniforms(this.idShader, {
    //   u_color: this.color!.toArray(),
    // })
    Asset.Shader.setCameraUniforms(this.idShader, this.camera!.getId())
    Asset.Shader.setCameraUniforms(this.idStaticShader, this.camera!.getId())
    Asset.Shader.setCameraUniforms(this.idAnimatedShader, this.camera!.getId())
    Asset.Shader.setCameraUniforms(this.idPhormShader, this.camera!.getId())
    Asset.Shader.setCameraUniforms(this.idPhormShader2, this.camera!.getId())

    Scene.copyTransform(this.idPhysics, this.idStaticInstance)
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

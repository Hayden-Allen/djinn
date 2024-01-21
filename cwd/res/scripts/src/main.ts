import "./lib/djinn.d"
import Camera from "./lib/Camera"
import Entity from "./lib/Entity"
import Color from "./lib/Color"
import TestEntity from "./TestEntity"
import GroundEntity from "./GroundEntity"
import Xport from "./lib/Xport"
import Player from "./Player"

const { Asset, Render, Nanovg, Scene, ImGui, Input, Sound } = djinn

export default class MainEntity extends Entity {
    private camera?: Camera
    private color: Color = new Color(0, 0, 0, 1)
    private entities: TestEntity[] = []
    private idShader: ShaderID
    private idTexture: TextureID
    private idMesh: MeshID
    private idSoundSource: SoundID
    private idSoundEmitter: SoundEmitterID
    private needsPlayAudio: boolean = true

    private idStaticMesh: MeshID
    private idStaticInstance: MeshInstanceID
    private idStaticShader: ShaderID

    private idAnimatedMesh: MeshID
    private idAnimatedInstances: MeshInstanceID[] = []
    private idAnimatedShader: ShaderID

    private frame: number = 0
    private nextAnimated: number = 0

    private idPhysics: PhysicsID
    private ground?: GroundEntity

    private idPhormShader: ShaderID
    private idPhormAlphaShader: ShaderID
    private xport?: Xport

    private player?: Player

    __init() {
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
        this.idSoundSource = Asset.Sound.load("white_out.mp3")
        this.idSoundEmitter = Scene.SoundEmitter.create(this.idSoundSource)
        Scene.setPos(this.idSoundEmitter, [-36, 103, -39])
        Scene.SoundEmitter.setSpatializationEnabled(this.idSoundEmitter, true)
        // Scene.SoundEmitter.setVolume(this.idSoundEmitter, 10)
        Scene.SoundEmitter.setLooping(this.idSoundEmitter, true)
        Scene.SoundEmitter.setRolloff(this.idSoundEmitter, 2)
        this.needsPlayAudio = true

        this.idStaticMesh = Asset.Mesh.loadStatic("cube.m3d")
        this.idStaticShader = Asset.Shader.load("static.vert", "static.frag")
        this.idStaticInstance = Scene.MeshInstance.create(
            this.idStaticMesh,
            this.idStaticShader
        )

        this.idAnimatedMesh = Asset.Mesh.loadAnimated("xbot.m3d")
        this.idAnimatedShader = Asset.Shader.load(
            "animated.vert",
            "animated.frag"
        )
        for (var i = 0; i < 10; i++) {
            this.idAnimatedInstances.push(
                Scene.MeshInstance.create(
                    this.idAnimatedMesh,
                    this.idAnimatedShader
                )
            )
            Scene.setPosX(this.idAnimatedInstances[i], -5 + i)
            Scene.setPosZ(this.idAnimatedInstances[i], -3)
        }
        Scene.setPos(this.idAnimatedInstances[0], [-36, 103, -39])

        this.idPhysics = Scene.Physics.createBox(1, [0, 50, 0], [1, 1, 1])

        this.ground = Scene.Entity.load("GroundEntity.js") as GroundEntity
        this.ground?.bind(this.camera)

        this.idPhormShader = Asset.Shader.load("phorm.vert", "phorm.frag")
        this.idPhormAlphaShader = Asset.Shader.load(
            "phorm.vert",
            "phorm_alpha.frag"
        )
        this.xport = new Xport("hill.xport")
        for (var i = 0; i < this.xport.idPhorms.length; i++) {
            Scene.Phorm.setShaders(this.xport.idPhorms[i], this.idPhormShader)
            Scene.Phorm.setAlphaShaders(
                this.xport.idPhorms[i],
                this.idPhormAlphaShader
            )
        }

        this.player = Scene.Entity.load("Player.js", this.camera!) as Player
    }
    __destroy() {
        Scene.MeshInstance.destroy(this.idStaticInstance)
        Asset.Mesh.destroy(this.idStaticMesh)
        Asset.Shader.destroy(this.idStaticShader)
        Scene.MeshInstance.destroyAll(this.idAnimatedInstances)
        Asset.Mesh.destroy(this.idAnimatedMesh)
        Asset.Shader.destroy(this.idAnimatedShader)

        Scene.SoundEmitter.stop(this.idSoundEmitter)
        Scene.SoundEmitter.destroy(this.idSoundEmitter)
        Asset.Sound.destroy(this.idSoundSource)
        Asset.Mesh.destroy(this.idMesh)
        Asset.Shader.destroy(this.idShader)
        Asset.Texture.destroy(this.idTexture)

        Scene.Physics.destroy(this.idPhysics)

        this.xport!.destroy()
        Asset.Shader.destroy(this.idPhormShader)
        Asset.Shader.destroy(this.idPhormAlphaShader)
    }
    __load() {
        this.color.set(0, 1, 1, 0.5)
        // for (var i = 0; i < 100; i++) {
        //   let e = Scene.Entity.load("TestEntity.js") as TestEntity
        //   e.bind(this.idMesh, this.idShader)
        //   this.entities.push(e)
        // }
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
                // 1 + this.nextAnimated / 10
                0.1
            )
            this.nextAnimated++
        }

        if (this.needsPlayAudio) {
            Scene.SoundEmitter.start(this.idSoundEmitter)
            this.needsPlayAudio = false
        }

        if (Input.getKey(Input.KEY_ENTER)) {
            Render.setDebugDrawEnabled(true)
        }
        if (Input.getKey(Input.KEY_BACKSPACE)) {
            Render.setDebugDrawEnabled(false)
        }
    }
    __draw() {
        this.xport!.skybox!.draw(this.camera!.getId())
        Asset.Shader.setCameraUniforms(this.idShader, this.camera!.getId())
        Asset.Shader.setCameraUniforms(
            this.idStaticShader,
            this.camera!.getId()
        )
        Asset.Shader.setCameraUniforms(
            this.idAnimatedShader,
            this.camera!.getId()
        )
        Asset.Shader.setCameraUniforms(this.idPhormShader, this.camera!.getId())
        Asset.Shader.setCameraUniforms(
            this.idPhormAlphaShader,
            this.camera!.getId()
        )

        Scene.copyTransform(this.idPhysics, this.idStaticInstance)
    }
    __ui() {
        // Nanovg.fillStyle(1, 1, 1)
        // Nanovg.fillRect(0, 0, 300, 300)
        // Nanovg.fillStyle(this.color.r, this.color.g, this.color.b)
        // Nanovg.fillRect(0, 0, 50, 50)
        // Nanovg.strokeStyle(1 - this.color.r, 1 - this.color.g, 1 - this.color.b)
        // Nanovg.strokeRect(50, 50, 50, 50)
        // Nanovg.strokeLine(50, 50, 100, 100)
        // Nanovg.strokeCircle(150, 150, 50)
        // Nanovg.fillCircle(200, 150, 50)
        // Nanovg.setFont("sans", 24)
        // Nanovg.drawText(100, 250, "TEST")
    }
    __imgui() {
        ImGui.text("I am a text box")
        this.color.fromArray(ImGui.colorPicker4("color", this.color.toArray()))
    }
}

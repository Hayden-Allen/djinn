import "./lib/djinn.d"
import Camera from "./lib/Camera"
import Entity from "./lib/Entity"
import Xport from "./lib/Xport"
import Player from "./Player"

const { Asset, Render, Scene, Input } = djinn

export default class MainEntity extends Entity {
    private camera?: Camera
    private idPhormShader: ShaderID
    private idPhormAlphaShader: ShaderID
    private idPhormWaterShader: ShaderID
    private xport?: Xport
    private player?: Player

    __init() {
        Asset.setDefaultTextureOptions({
            minFilter: Render.GL_LINEAR_MIPMAP_LINEAR,
            magFilter: Render.GL_LINEAR,
        })
        
        this.camera = Scene.Camera.load("lib/Camera.js") as Camera
        const ar = Render.getAspectRatio()
        Scene.Camera.configure(this.camera.getId(), 108 / ar, ar, 0.1, 1000)

        this.xport = new Xport("01.xport")
        this.idPhormShader = Asset.Shader.load("phorm.vert", "phorm.frag")
        this.idPhormAlphaShader = Asset.Shader.load(
            "phorm.vert",
            "phorm_alpha.frag"
        )
        this.idPhormWaterShader = Asset.Shader.load(
            "phorm_water.vert",
            "phorm_water.frag"
        )       
        for (var i = 0; i < this.xport.idPhorms.length; i++) {
            const id = this.xport.idPhorms[i]
            Scene.Phorm.setShaders(id, this.idPhormShader)
            Scene.Phorm.setAlphaShaders(id, this.idPhormAlphaShader)
        }
        
        this.player = Scene.Entity.load("Player.js", this.camera!) as Player
    }
    __destroy() {
        this.xport!.destroy()
        Asset.Shader.destroy(this.idPhormShader)
        Asset.Shader.destroy(this.idPhormAlphaShader)
        Asset.Shader.destroy(this.idPhormWaterShader)
    }
    __main(dt: number, time: number) {
        if (Input.getKey(Input.KEY_ENTER)) {
            Render.setDebugDrawEnabled(true)
        }
        if (Input.getKey(Input.KEY_BACKSPACE)) {
            Render.setDebugDrawEnabled(false)
        }
    }
    __draw() {
        this.xport!.skybox!.draw(this.camera!.getId())
        Asset.Shader.setCameraUniforms(this.idPhormShader, this.camera!.getId())
        Asset.Shader.setCameraUniforms(
            this.idPhormAlphaShader,
            this.camera!.getId()
        )
        Asset.Shader.setCameraUniforms(
            this.idPhormWaterShader,
            this.camera!.getId()
        )
    }
}

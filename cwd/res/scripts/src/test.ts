import './globals.d'
import { Entity } from './lib'

const { Asset, Render, Nanovg } = djinn

export default class TestClass extends Entity
{
    private startTime: number = 0
    private idMesh: number = -1
    private idShader: number = -1
    private idTexture: number = -1
    __load()
    {
        this.startTime = Date.now()

        this.idMesh = Asset.Mesh.create(4, [2, 2], 6)
        Asset.Mesh.update(this.idMesh,
            [
                0, 0, 0, 0,
                1, 0, 1, 0,
                1, 1, 1, 1,
                0, 1, 0, 1,
            ],
            [0, 1, 2, 0, 2, 3]
        )

        this.idShader = Asset.Shader.load('test.vert', 'test.frag')

        this.idTexture = Asset.Texture.load("test.bmp",
        {
            'minFilter': GL_NEAREST,
            'magFilter': GL_LINEAR,
        })
    }
    __unload()
    {
        Asset.Mesh.destroy(this.idMesh)
        Asset.Shader.destroy(this.idShader)
        Asset.Texture.destroy(this.idTexture)
    }
    __main()
    {
    }
    __draw()
    {
        Asset.Shader.setUniforms(this.idShader, {
            // u_color1: [1, 0, 0, 1],
            // u_color2: [0, 0, 1, 1],
            // u_colorMix: Math.abs(Math.cos((Date.now() - this.startTime) * 0.001)),
            u_texture: [this.idTexture, 0]
        })
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

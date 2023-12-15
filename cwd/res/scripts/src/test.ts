import { Entity } from './lib'

const { Asset, Render } = djinn

export default class TestClass extends Entity
{
    private startTime: number = 0
    private idMesh: number
    private idShader: number
    private idTexture: number
    genTexture()
    {
        const TW = 32, TH = 32
        let pixels = new Array(TW * TH * 4).fill(0)
        for (let y = 0; y < TH; y++)
        {
            for (let x = 0; x < TW; x++)
            {
                const off = y * (TW * 4) + x * 4
                pixels[off + 0] = Math.round(255 * ((TW - x) / (TW * 2) + (TH - y) / (TH * 2)))
                pixels[off + 1] = 0;
                pixels[off + 2] = 0;
                pixels[off + 3] = 255;
            }
        }
        return pixels
    }
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

        this.idTexture = Asset.Texture.create(32, 32)
        Asset.Texture.update(this.idTexture, this.genTexture(),
        {
            'minFilter': 0x2600,
            'magFilter': 0x2600,
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
        Asset.Shader.setUniforms(this.idShader, {
            // u_color1: [1, 0, 0, 1],
            // u_color2: [0, 0, 1, 1],
            // u_colorMix: Math.abs(Math.cos((Date.now() - this.startTime) * 0.001)),
            u_texture: [this.idTexture, 0]
        })
        Render.draw(this.idMesh, this.idShader)
    }
}

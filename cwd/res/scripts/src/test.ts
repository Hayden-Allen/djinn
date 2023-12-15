import { Entity } from './lib'

const { Asset, Render } = djinn

export default class TestClass extends Entity
{
    private startTime: number = 0
    private idMesh: number
    private idShader: number
    __load()
    {
        this.startTime = Date.now()
        this.idMesh = Asset.Mesh.create(4, [2], 6)
        Asset.Mesh.update(this.idMesh, [0, 0, 1, 0, 1, 1, 0, 1], [0, 1, 2, 0, 2, 3])
        this.idShader = Asset.Shader.load('test.vert', 'test.frag')
    }
    __unload()
    {
        Asset.Mesh.destroy(this.idMesh)
        Asset.Shader.destroy(this.idShader)
    }
    __main()
    {
        Asset.Shader.setUniforms(this.idShader, {
            u_color1: [1, 0, 0, 1],
            u_color2: [0, 0, 1, 1],
            u_colorMix: Math.abs(Math.cos((Date.now() - this.startTime) * 0.001)),
        })
        Render.draw(this.idMesh, this.idShader)
    }
}

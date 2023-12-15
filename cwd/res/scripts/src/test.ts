import { cool, Entity } from './lib'

const { Asset, Render } = djinn

const vert_src = `
    #version 330 core
    layout(location = 0) in vec2 i_pos;
    void main() {
        gl_Position = vec4(i_pos, 0, 1);
    }
`
const frag_src = `
    #version 330 core
    layout(location = 0) out vec4 o_color;
    uniform vec4 u_color1;
    uniform vec4 u_color2;
    uniform float u_colorMix;
    void main() {
        o_color = mix(u_color1, u_color2, u_colorMix);
    }
`

export default class TestClass extends Entity
{
    private startTime: number = 0;
    private idMesh: number;
    private idShader: number;
    __load()
    {
        this.startTime = Date.now();
        this.idMesh = Asset.Mesh.create(4, [2], 6);
        Asset.Mesh.update(this.idMesh, [0, 0, 1, 0, 1, 1, 0, 1], [0, 1, 2, 0, 2, 3]);
        this.idShader = Asset.Shader.create(vert_src, frag_src);
    }
    __unload()
    {
        Asset.Mesh.destroy(this.idMesh);
        Asset.Shader.destroy(this.idShader);
    }
    __main()
    {
        Asset.Shader.setUniforms(this.idShader, {
            u_color1: [1, 0, 0, 1],
            u_color2: [0, 0, 1, 1],
            u_colorMix: Math.abs(Math.cos((Date.now() - this.startTime) * 0.001)),
        });
        Render.draw(this.idMesh, this.idShader);
    }
}

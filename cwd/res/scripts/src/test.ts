import { cool } from './lib'

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
    void main() {
        o_color = vec4(1);
    }
`

export default class TestClass
{
    private x: number = 0;
    private lastTime: number = 0;
    private ro_id: number;
    private shader_id: number;
    __load()
    {
        this.ro_id = create_mesh(4, [2], 6);
        this.shader_id = create_shader(vert_src, frag_src);
        console.log(this.ro_id, this.shader_id);
    }
    __unload()
    {
        console.log("RELOAD");
        destroy_mesh(this.ro_id);
    }
    __main()
    {
        const now : number = Date.now();
        if(now - this.lastTime > 1000)
        {
            update_mesh(this.ro_id, [this.x, 0, this.x + 1, 0, this.x + 1, 1, this.x, 1], [0, 1, 2, 0, 2, 3]);
            this.lastTime = now;
            this.x += .1;
        }
        draw(this.ro_id, this.shader_id);
    }
}
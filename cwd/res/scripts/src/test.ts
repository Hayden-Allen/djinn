import { cool } from './lib'

export default class TestClass
{
    public x: number = 0;
    public lastTime: number = 0;
    __load()
    {
        this.x = 0;
        this.lastTime = 0;
    }
    __unload()
    {
        console.log("RELOAD");
    }
    __main()
    {
        const now : number = Date.now();
        if(now - this.lastTime > 1000)
        {
            // console.log("b", String.fromCharCode(this.x++), cool(this.x));
            let id = create_mesh(4, [3], 6);
            console.log(id);
            destroy_mesh(id);
            this.lastTime = now;
        }
    }
}
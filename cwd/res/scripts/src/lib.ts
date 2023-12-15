export function cool(x: number) : number
{
    const y: number = 0xffffffff - x;
    return y + x;
}
export class Entity
{
    private id: number = -1;
    __init() {}
    __destroy() {}
    __load() {}
    __unload() {}
    __main() {}
}

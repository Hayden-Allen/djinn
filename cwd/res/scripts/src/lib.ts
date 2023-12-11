export function cool(x: number) : number
{
    const y: number = 0xffffffff - x;
    return y + x;
}
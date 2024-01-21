declare interface _Sound {
    setListenerWorldPos(worldPos: number[]): void
    setListenerWorldDir(worldDir: number[]): void

    ATTENUATION_INVERSE: number
    ATTENUATION_LINEAR: number
    ATTENUATION_EXPONENTIAL: number
}

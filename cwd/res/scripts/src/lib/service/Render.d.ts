declare interface _Render {
    getAspectRatio(): number
    setDebugDrawEnabled(enabled: boolean): void

    GL_NEAREST: number
    GL_LINEAR: number
    GL_REPEAT: number
}

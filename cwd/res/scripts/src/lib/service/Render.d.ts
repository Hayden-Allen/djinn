declare interface _Render {
    getAspectRatio(): number
    setDebugDrawEnabled(enabled: boolean): void

    GL_NEAREST: number
    GL_LINEAR: number
    GL_NEAREST_MIPMAP_NEAREST: number
    GL_LINEAR_MIPMAP_NEAREST: number
    GL_NEAREST_MIPMAP_LINEAR: number
    GL_LINEAR_MIPMAP_LINEAR: number
    GL_REPEAT: number
}

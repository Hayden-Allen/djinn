declare interface _Render {
  // bindTexture(id: number, slot: number): void
  // bindCubemap(id: number, slot: number): void
  getAspectRatio(): number

  GL_NEAREST: number
  GL_LINEAR: number
  GL_REPEAT: number
}

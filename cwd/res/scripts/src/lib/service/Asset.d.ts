import "./id.d"

declare interface _Asset_Mesh {
  create(
    vertCount: number,
    layout: number[],
    idxCount: number,
    idTextures: TextureID[]
  ): MeshID
  update(id: MeshID, verts: number[], indices: number[]): void
  destroy(id: MeshID): void
  loadStatic(fp: string): MeshID
  loadAnimated(fp: string): MeshID
}
declare interface _Asset_Shader {
  load(fpVert: string, fpFrag: string): ShaderID
  setUniforms(id: ShaderID, uniforms: object): void
  setCameraUniforms(idShader: ShaderID, idCamera: CameraID): void
  destroy(id: ShaderID): void
}
declare interface _Asset_Texture {
  create(width: number, height: number, options?: object): TextureID
  load(fp: string, options?: object): TextureID
  update(id: TextureID, subpixels: number[], options?: object): void
  destroy(id: TextureID): void
}
declare interface _Asset_Cubemap {
  create(width: number, height: number, options?: object): CubemapID
  load(fps: string[], options?: object): CubemapID
  update(id: CubemapID, subpixels: number[][], options?: object): void
  destroy(id: CubemapID): void
}
declare interface _Asset_Sound {
  load(fp: string): SoundID
  destroy(id: SoundID): void
}
export declare interface _Asset {
  Mesh: _Asset_Mesh
  Shader: _Asset_Shader
  Texture: _Asset_Texture
  Cubemap: _Asset_Cubemap
  Sound: _Asset_Sound
}

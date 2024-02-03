import "./id.d"

declare interface _Asset_Mesh {
    create(
        vertCount: number,
        layout: number[],
        idxCount: number,
        idTextures: (TextureID | CubemapID)[]
    ): MeshID
    update(id: MeshID, verts: number[], indices: number[]): void
    updateVertices(id: MeshID, verts: number[]): void
    updateIndices(id: MeshID, indices: number[]): void
    loadStatic(fp: string): MeshID
    loadAnimated(fp: string): MeshID
    destroy(id: MeshID): void
    destroyAll(ids: MeshID[]): void
}
declare interface _Asset_Shader {
    load(fpVert: string, fpFrag: string): ShaderID
    setUniforms(id: ShaderID, uniforms: Record<string, number>): void
    setCameraUniforms(idShader: ShaderID, idCamera: CameraID): void
    destroy(id: ShaderID): void
    destroyAll(ids: ShaderID[]): void
}
declare interface _Asset_Texture {
    create(
        width: number,
        height: number,
        options?: Record<string, number>
    ): TextureID
    load(fp: string, options?: Record<string, number>): TextureID
    update(
        id: TextureID,
        subpixels: number[],
        options?: Record<string, number>
    ): void
    setOptions(id: TextureID, options: Record<string, number>): void
    destroy(id: TextureID): void
    destroyAll(ids: TextureID[]): void
}
declare interface _Asset_Cubemap {
    create(
        width: number,
        height: number,
        options?: Record<string, number>
    ): CubemapID
    load(fps: string[], options?: Record<string, number>): CubemapID
    update(
        id: CubemapID,
        subpixels: number[][],
        options?: Record<string, number>
    ): void
    destroy(id: CubemapID): void
    destroyAll(ids: CubemapID[]): void
}
declare interface _Asset_Sound {
    load(fp: string): SoundID
    destroy(id: SoundID): void
    destroyAll(ids: SoundID[]): void
}
export declare interface _Asset {
    Mesh: _Asset_Mesh
    Shader: _Asset_Shader
    Texture: _Asset_Texture
    Cubemap: _Asset_Cubemap
    Sound: _Asset_Sound

    setDefaultTextureOptions(options: Record<string, number>): void
}

import "./globals.d"

const { Asset, Scene, Util } = djinn

interface SkyboxTextureOptions {
  minFilter: number
  magFilter: number
}

interface SkyboxLoadFilesOptions {
  front: string
  back: string
  left: string
  right: string
  top: string
  bottom: string
  textureOptions: SkyboxTextureOptions
}

interface SkyboxLoadDirectoryOptions {
  dir: string
  textureOptions: SkyboxTextureOptions
}

interface SkyboxGeneratedOptions {
  width: number
  height: number
  pixels: Array<Array<number>>
  textureOptions: SkyboxTextureOptions
}

interface SkyboxOptions {
  vertexShader: string
  fragmentShader: string
}

export default class Skybox {
  private idMesh: number = 0
  private idInstance: number = 0
  private idShader: number = 0
  private idTexture: number = 0

  constructor(idTexture: number, options: SkyboxOptions) {
    this.idTexture = idTexture
    this.idShader = Asset.Shader.load(
      options.vertexShader,
      options.fragmentShader
    )
    this.idMesh = Asset.Mesh.create(8, [3], 36, [this.idTexture], this.idShader)
    Asset.Mesh.update(
      this.idMesh,
      [
        // 0 - fbl
        -1.0, -1.0, 1.0,
        // 1 - fbr
        1.0, -1.0, 1.0,
        // 2 - ftr
        1.0, 1.0, 1.0,
        // 3 - ftl
        -1.0, 1.0, 1.0,
        // 4 - bbl
        -1.0, -1.0, -1.0,
        // 5 - bbr
        1.0, -1.0, -1.0,
        // 6 - btr
        1.0, 1.0, -1.0,
        // 7 - btl
        -1.0, 1.0, -1.0,
      ],
      [
        // front
        0, 2, 1, 0, 3, 2,
        // right
        1, 6, 5, 1, 2, 6,
        // back
        5, 7, 4, 5, 6, 7,
        // left
        4, 3, 0, 4, 7, 3,
        // top
        3, 6, 2, 3, 7, 6,
        // bottom
        4, 1, 5, 4, 0, 1,
      ]
    )
    this.idInstance = Scene.MeshInstance.create(this.idMesh)

    Asset.Shader.setUniforms(this.idShader, {
      u_texture: 0,
    })
  }

  static loadFiles(
    sboxOptions: SkyboxOptions,
    loadOptions: SkyboxLoadFilesOptions
  ) {
    const fps = [
      loadOptions.right,
      loadOptions.left,
      loadOptions.top,
      loadOptions.bottom,
      loadOptions.back,
      loadOptions.front,
    ]
    const idTexture = Asset.Cubemap.load(fps, loadOptions.textureOptions)
    const ret = new Skybox(idTexture, sboxOptions)
    return ret
  }

  static loadDirectory(
    sboxOptions: SkyboxOptions,
    loadOptions: SkyboxLoadDirectoryOptions
  ) {
    const fps = Util.listFiles(Util.makeTexturePath(loadOptions.dir))
    const idTexture = Asset.Cubemap.load(fps, loadOptions.textureOptions)
    const ret = new Skybox(idTexture, sboxOptions)
    return ret
  }

  static createGenerated(
    sboxOptions: SkyboxOptions,
    genOptions: SkyboxGeneratedOptions
  ) {
    const idTexture = Asset.Cubemap.create(
      genOptions.width,
      genOptions.height,
      genOptions.textureOptions
    )
    Asset.Cubemap.update(idTexture, genOptions.pixels)
    const ret = new Skybox(idTexture, sboxOptions)
    return ret
  }

  destroy() {
    Asset.Mesh.destroy(this.idMesh)
    Asset.Mesh.destroyInstance(this.idInstance)
    Asset.Shader.destroy(this.idShader)
    Asset.Cubemap.destroy(this.idTexture)
  }

  draw(idCamera: number) {
    // TODO: glDepthMask(false);
    // Render.bindCubemap(this.idTexture, 0)
    Asset.Shader.setCameraUniforms(this.idShader, idCamera)
    // Render.draw(this.idMesh, this.idShader)
  }
}

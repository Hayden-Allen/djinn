import "./globals.d"

const { Asset, Render } = djinn

interface SkyboxTextureOptions {
  minFilter: number
  magFilter: number
}

interface SkyboxTexturePaths {
  front: string
  back: string
  left: string
  right: string
  top: string
  bottom: string
}

interface SkyboxOptions {
  vertexShader: string
  fragmentShader: string
  textures: SkyboxTexturePaths
  textureOptions: SkyboxTextureOptions
}

function genTexture(
  w: number,
  h: number,
  rmask: number,
  gmask: number,
  bmask: number
) {
  const pixels = new Array(w * h * 4)
  for (let y = 0; y < h; y++) {
    for (let x = 0; x < w; x++) {
      const off = y * (w * 4) + x * 4
      const val = Math.round(255 * (x / (w * 2) + y / (h * 2)))
      pixels[off + 0] = rmask * val
      pixels[off + 1] = gmask * val
      pixels[off + 2] = bmask * val
      pixels[off + 3] = 255
    }
  }
  return pixels
}

export default class Skybox {
  private idMesh: number = -1
  private idShader: number = -1
  private idTexture: number = -1

  constructor(options: SkyboxOptions) {
    this.idMesh = Asset.Mesh.create(8, [3], 36)
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

    this.idShader = Asset.Shader.load(
      options.vertexShader,
      options.fragmentShader
    )

    // const fps = [
    //   options.textures.right,
    //   options.textures.left,
    //   options.textures.top,
    //   options.textures.bottom,
    //   options.textures.back,
    //   options.textures.front,
    // ]
    // this.idTexture = Asset.Cubemap.load(fps, options.textureOptions)

    const TW = 32,
      TH = 32
    this.idTexture = Asset.Cubemap.create(32, 32, options.textureOptions)
    const pixels = []
    const R_MASK = [0, 0, 0, 0, 1, 1]
    const G_MASK = [0, 0, 1, 1, 0, 0]
    const B_MASK = [0, 1, 0, 1, 0, 1]
    for (let i = 0; i < 6; i++) {
      pixels[i] = genTexture(TW, TH, R_MASK[i], G_MASK[i], B_MASK[i])
    }
    Asset.Cubemap.update(this.idTexture, pixels)

    Asset.Shader.setUniforms(this.idShader, {
      u_texture: 0,
    })
  }

  unload() {
    Asset.Mesh.destroy(this.idMesh)
    Asset.Shader.destroy(this.idShader)
    Asset.Cubemap.destroy(this.idTexture)
  }

  draw() {
    // TODO: glDepthMask(false);
    Render.bindCubemap(this.idTexture, 0)
    Render.draw(this.idMesh, this.idShader)
  }
}

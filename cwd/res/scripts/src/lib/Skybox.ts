const { Asset, Render, Nanovg } = djinn;

interface SkyboxTextureOptions {
  minFilter: number;
  magFilter: number;
}

interface SkyboxTexturePaths {
  front: string;
  back: string;
  left: string;
  right: string;
  top: string;
  bottom: string;
}

interface SkyboxOptions {
  vertexShader: string;
  fragmentShader: string;
  textures: SkyboxTexturePaths;
  textureOptions: SkyboxTextureOptions;
}

export default class Skybox {
  private idMesh: number = -1;
  private idShader: number = -1;
  private idTexture: number = -1;

  constructor(options: SkyboxOptions) {
    this.idMesh = Asset.Mesh.create(8, [3], 36);
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
    );

    this.idShader = Asset.Shader.load(
      options.vertexShader,
      options.fragmentShader
    );

    this.idTexture = Asset.Cubemap.load(options.textures, {
      minFilter: options.textureOptions.minFilter,
      magFilter: options.textureOptions.magFilter,
    });

    Asset.Shader.setUniforms(this.idShader, {
      u_texture: [this.idTexture, 0],
    });
  }

  unload() {
    Asset.Mesh.destroy(this.idMesh);
    Asset.Shader.destroy(this.idShader);
    Asset.Texture.destroy(this.idTexture);
  }

  draw() {
    Render.draw(this.idMesh, this.idShader);
  }
}

declare type Optional<T> = T | undefined

declare interface _Asset_Mesh {
  create(
    vertCount: number,
    layout: number[],
    idxCount: number,
    idTextures: number[]
  ): number
  update(id: number, verts: number[], indices: number[]): void
  destroy(id: number): void
  loadStatic(fp: string): number
  loadAnimated(fp: string): number
}
declare interface _Asset_Shader {
  load(fpVert: string, fpFrag: string): number
  destroy(id: number): void
  setUniforms(id: number, uniforms: object): void
  setCameraUniforms(idShader: number, idCamera: number): void
}
declare interface _Asset_Texture {
  create(width: number, height: number, options?: object): number
  load(fp: string, options?: object): number
  destroy(id: number): void
  update(id: number, subpixels: number[], options?: object): void
}
declare interface _Asset_Cubemap {
  create(width: number, height: number, options?: object): number
  load(fps: string[], options?: object): number
  destroy(id: number): void
  update(id: number, subpixels: number[][], options?: object): void
}
declare interface _Asset_Sound {
  load(fp: string): number
  destroy(id: number): void
}
declare interface _Asset {
  Mesh: _Asset_Mesh
  Shader: _Asset_Shader
  Texture: _Asset_Texture
  Cubemap: _Asset_Cubemap
  Sound: _Asset_Sound
}

declare interface _ImGui {
  text(text: string): void
  colorPicker4(label: string, colors: number[]): number[]
}

declare interface _Input {
  getKey(key: number): number
  leftX(): number
  leftY(): number
  rightX(): number
  rightY(): number
  KEY_0: number
  KEY_1: number
  KEY_2: number
  KEY_3: number
  KEY_4: number
  KEY_5: number
  KEY_6: number
  KEY_7: number
  KEY_8: number
  KEY_9: number
  KEY_A: number
  KEY_B: number
  KEY_C: number
  KEY_D: number
  KEY_E: number
  KEY_F: number
  KEY_G: number
  KEY_H: number
  KEY_I: number
  KEY_J: number
  KEY_K: number
  KEY_L: number
  KEY_M: number
  KEY_N: number
  KEY_O: number
  KEY_P: number
  KEY_Q: number
  KEY_R: number
  KEY_S: number
  KEY_T: number
  KEY_U: number
  KEY_V: number
  KEY_W: number
  KEY_X: number
  KEY_Y: number
  KEY_Z: number
  KEY_RIGHT: number
  KEY_LEFT: number
  KEY_DOWN: number
  KEY_UP: number
  KEY_SPACE: number
  KEY_SHIFT: number
}

declare interface _Nanovg {
  fillStyle(r: number, g: number, b: number, a?: number): void
  fillRect(x: number, y: number, w: number, h: number): void
  fillArc(
    x: number,
    y: number,
    r: number,
    start: number,
    end: number,
    dir: number
  ): void
  fillCircle(x: number, y: number, r: number): void
  strokeStyle(r: number, g: number, b: number, a?: number): void
  strokeLine(x0: number, y0: number, x1: number, y1: number): void
  strokeRect(x: number, y: number, w: number, h: number): void
  strokeArc(
    x: number,
    y: number,
    r: number,
    start: number,
    end: number,
    dir: number
  ): void
  strokeCircle(x: number, y: number, r: number): void
  setFont(name: string, size: number): void
  drawText(x: number, y: number, msg: string): void
}

declare interface _Render {
  bindTexture(id: number, slot: number): void
  bindCubemap(id: number, slot: number): void
  getAspectRatio(): number

  GL_NEAREST: number
  GL_LINEAR: number
  GL_REPEAT: number
}

declare interface _Scene_MeshInstance {
  create(idMesh: number, idShader: number): number
  setUniforms(id: number, map: object): void
  setAction(id: number, name: string, speed?: number): void
  setVisible(id: number, visible: boolean): void
  destroy(id: number): void
}
declare interface _Scene_Entity {
  load(fp: string): object
  requestImGui(id: number): void
  destroy(id: number): void
}
declare interface _Scene_Camera {
  load(fp: string): object
  configure(
    id: number,
    fovy: number,
    aspect: number,
    near: number,
    far: number
  ): void
}
declare interface _Scene_Phorm {
  load(fp: string): number[]
  setShaders(idPhorm: number, idShader: number): void
  setVisible(id: number, visible: boolean): void
  destroy(id: number): void
}
declare interface _Scene_Physics {
  create(dims: number[], origin: number[], mass: number): number
  setFriction(id: number, f: number): void
  setLinearVelocity(id: number, vel: number[]): void
  setAngularVelocity(id: number, vel: number[]): void
  destroy(id: number): void
}
declare interface _Scene_Light {
  create(): number
  destroy(id: number): void
}
declare interface _Scene {
  MeshInstance: _Scene_MeshInstance
  Entity: _Scene_Entity
  Camera: _Scene_Camera
  Phorm: _Scene_Phorm
  Physics: _Scene_Physics
  Light: _Scene_Light

  copyTransform(idFrom: number, idTo: number): void
  getPos(id: number): number[]
  getPosX(id: number): number
  getPosY(id: number): number
  getPosZ(id: number): number
  setPos(id: number, xyz: number[]): void
  setPosX(id: number, x: number): void
  setPosY(id: number, y: number): void
  setPosZ(id: number, z: number): void
  addPos(id: number, xyz: number[]): void
  addPosX(id: number, x: number): void
  addPosY(id: number, y: number): void
  addPosZ(id: number, z: number): void
  addPosLocal(id: number, xyz: number[]): void
  addPosLocalX(id: number, x: number): void
  addPosLocalY(id: number, y: number): void
  addPosLocalZ(id: number, z: number): void

  getRot(id: number): number[]
  getRotX(id: number): number
  getRotY(id: number): number
  getRotZ(id: number): number
  setRot(id: number, xyz: number[]): void
  setRotX(id: number, x: number): void
  setRotY(id: number, y: number): void
  setRotZ(id: number, z: number): void
  addRot(id: number, xyz: number[]): void
  addRotX(id: number, x: number): void
  addRotY(id: number, y: number): void
  addRotZ(id: number, z: number): void
  addRotLocal(id: number, xyz: number[]): void
  addRotLocalX(id: number, x: number): void
  addRotLocalY(id: number, y: number): void
  addRotLocalZ(id: number, z: number): void

  getScale(id: number): number[]
  getScaleX(id: number): number
  getScaleY(id: number): number
  getScaleZ(id: number): number
  setScale(id: number, xyz: number[]): void
  setScaleX(id: number, x: number): void
  setScaleY(id: number, y: number): void
  setScaleZ(id: number, z: number): void
  addScale(id: number, xyz: number[]): void
  addScaleX(id: number, x: number): void
  addScaleY(id: number, y: number): void
  addScaleZ(id: number, z: number): void
  addScaleLocal(id: number, xyz: number[]): void
  addScaleLocalX(id: number, x: number): void
  addScaleLocalY(id: number, y: number): void
  addScaleLocalZ(id: number, z: number): void
}

declare interface _Sound_Emitter {
  create(idSource: number): number
  play(id: number): void
  stop(id: number): void
  destroy(id: number): void
}
declare interface _Sound {
  Emitter: _Sound_Emitter
}

declare interface _Util {
  makeTexturePath(relPath: string): string
  listFiles(absolutePath: string): string[]
}

declare interface _Djinn {
  Asset: _Asset
  ImGui: _ImGui
  Input: _Input
  Nanovg: _Nanovg
  Render: _Render
  Scene: _Scene
  Sound: _Sound
  Util: _Util
}

declare var djinn: _Djinn

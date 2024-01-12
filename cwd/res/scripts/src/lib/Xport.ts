import "./globals.d"

const { Asset, Scene } = djinn

export default class Xport {
  idTextures: number[] = []
  idPhorms: number[] = []
  idLights: number[] = []
  idWaypoints: number[] = []

  constructor(fp: string) {
    const { textures, phorms, lights, waypoints } = Scene.Xport.load(fp)
    this.idTextures = textures
    this.idPhorms = phorms
    this.idLights = lights
    this.idWaypoints = waypoints
  }
  destroy() {
    for (const id of this.idTextures) Asset.Texture.destroy(id)
    for (const id of this.idPhorms) Scene.Phorm.destroy(id)
    for (const id of this.idLights) Scene.Light.destroy(id)
    for (const id of this.idWaypoints) Scene.Waypoint.destroy(id)
  }
}

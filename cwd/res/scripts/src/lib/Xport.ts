import "./globals.d"

const { Scene } = djinn

export default class Xport {
  idPhorms: number[] = []
  idLights: number[] = []
  idWaypoints: number[] = []

  constructor(fp: string) {
    const { phorms, lights, waypoints } = Scene.Xport.load(fp)
    this.idPhorms = phorms
    this.idLights = lights
    this.idWaypoints = waypoints
  }
  destroy() {
    for (const id of this.idPhorms) Scene.Phorm.destroy(id)
    for (const id of this.idLights) Scene.Light.destroy(id)
    for (const id of this.idWaypoints) Scene.Waypoint.destroy(id)
  }
}

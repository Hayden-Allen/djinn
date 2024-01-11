import "./globals.d"

const { Scene } = djinn

export default class Xport {
  idPhorms: number[] = []
  idLights: number[] = []

  constructor(fp: string) {
    const { phorms, lights } = Scene.Xport.load(fp)
    this.idPhorms = phorms
    this.idLights = lights
  }
  destroy() {
    for (const id of this.idPhorms) Scene.Phorm.destroy(id)
    for (const id of this.idLights) Scene.Light.destroy(id)
  }
}

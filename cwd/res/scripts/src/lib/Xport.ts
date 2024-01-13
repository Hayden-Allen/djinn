import "./djinn.d"
import Skybox from "./Skybox"

const { Asset, Scene } = djinn

export default class Xport {
  idTextures: TextureID[] = []
  idPhorms: PhormID[] = []
  idLights: LightID[] = []
  idWaypoints: WaypointID[] = []
  idHitboxes: PhysicsID[] = []
  skybox: Optional<Skybox>

  constructor(fp: string) {
    const { textures, skybox, phorms, lights, waypoints } = Scene.Xport.load(fp)
    this.idTextures = textures
    this.idPhorms = phorms
    this.idLights = lights
    this.idWaypoints = waypoints
    for (const id of this.idPhorms) {
      this.idHitboxes.push(Scene.Physics.createBVH(id))
    }
    this.skybox = new Skybox(skybox, {
      vertexShader: "sky.vert",
      fragmentShader: "sky.frag",
    })
  }
  destroy() {
    console.log("A")
    for (const id of this.idTextures) Asset.Texture.destroy(id)
    console.log("B")
    for (const id of this.idPhorms) Scene.Phorm.destroy(id)
    for (const id of this.idLights) Scene.Light.destroy(id)
    for (const id of this.idWaypoints) Scene.Waypoint.destroy(id)
    for (const id of this.idHitboxes) Scene.Physics.destroy(id)
    console.log("C")
    this.skybox!.destroy()
    console.log("D")
  }
}

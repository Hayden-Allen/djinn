import "./djinn.d"
import Skybox from "./Skybox"

const { Asset, Scene } = djinn

export default class Xport {
    idTextures: TextureID[] = []
    idPhorms: PhormID[] = []
    idLights: LightID[] = []
    idWaypoints: WaypointID[] = []
    idHitboxes: PhysicsID[] = []
    skybox?: Skybox

    constructor(fp: string) {
        const { textures, skybox, phorms, lights, waypoints } =
            Scene.Xport.load(fp)
        this.idTextures = textures
        this.idPhorms = phorms
        this.idLights = lights
        this.idWaypoints = waypoints
        for (const idPhorm of this.idPhorms) {
            const idHitbox = Scene.Physics.createBVH(idPhorm)
            this.idHitboxes.push(idHitbox)
            Scene.Physics.bind(idHitbox, idPhorm)
            if (Scene.Tag.has(idPhorm, "trigger")) {
                Scene.Physics.setGhost(idHitbox, true)
                Scene.Phorm.setVisible(idPhorm, false)
            }
        }
        this.skybox = new Skybox(skybox, {
            vertexShader: "sky.vert",
            fragmentShader: "sky.frag",
        })
    }
    destroy() {
        Asset.Texture.destroyAll(this.idTextures)
        Scene.Phorm.destroyAll(this.idPhorms)
        Scene.Light.destroyAll(this.idLights)
        Scene.Waypoint.destroyAll(this.idWaypoints)
        Scene.Physics.destroyAll(this.idHitboxes)
        this.skybox!.destroy()
    }
}

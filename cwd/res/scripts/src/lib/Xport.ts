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
    tag2Phorm: Map<string, Set<PhormID>> = new Map()
    name2Phorm: Map<string, PhormID> = new Map()

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
            Scene.setParent(idHitbox, idPhorm)
            Scene.Physics.bind(idHitbox, idPhorm)
            if (Scene.Tag.has(idPhorm, "trigger")) {
                Scene.Physics.setGhost(idHitbox, true)
                Scene.Phorm.setVisible(idPhorm, false)
            }
            const tags = Scene.Tag.get(idPhorm)
            for (const tag of tags) {
                if (!this.tag2Phorm.has(tag)) {
                    this.tag2Phorm.set(tag, new Set())
                }
                this.tag2Phorm.get(tag)?.add(idPhorm)
            }
            this.name2Phorm.set(Scene.Xport.getName(idPhorm), idPhorm)
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
    getPhormByName(name: string) {
        return this.name2Phorm.get(name)
    }
}

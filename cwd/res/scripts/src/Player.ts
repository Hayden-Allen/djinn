import "./lib/djinn.d"
import Entity from "./lib/Entity"
import Camera from "./lib/Camera"
import SoundEmitter from "./lib/wrap/SoundEmitter"
import { StateNode, StateGraph } from "./lib/wrap/StateGraph"

const { Asset, Event, Scene, Input, ImGui, Sound } = djinn

export default class Player extends Entity {
    private camera?: Camera
    private idHitbox: PhysicsID
    private camAngleX: number = 0
    private camAngleY: number = 0
    private hitboxHeight: number = 0.1
    private hitboxRadius: number = 0.1
    private worldPos: number[] = [0, 0, 0]

    private canJump: boolean = false
    private velY: number = 0
    private velYMin: number = -100
    private velYMax: number = 20
    private gravity: number = 65
    private walkSpeed: number = 3
    private runSpeed: number = 5

    private soundWalk?: SoundEmitter
    private soundRun?: SoundEmitter
    private soundJump?: SoundEmitter

    private idSphereMesh: MeshID = 0 as MeshID
    private idSphereInstances: MeshInstanceID[] = []
    private idSphereShader: ShaderID = 0 as ShaderID

    private state: StateGraph = new StateGraph()

    __init(cam: Camera) {
        if (cam) {
            this.camera = cam
        }
    }
    __load() {
        ImGui.setName(this.id, "Player")
        // hitbox
        {
            this.idHitbox = Scene.Physics.createCapsuleY(
                1,
                this.worldPos,
                this.hitboxRadius,
                this.hitboxHeight
            )
            Scene.Physics.bind(this.idHitbox, this.id)

            Scene.Physics.setGravity(this.idHitbox, [0, 0, 0])
            Scene.Physics.setAngularFactor(this.idHitbox, [0, 0, 0])
        }
        // scene graph
        {
            if (this.camera) {
                const idCam = this.camera!.getId()
                Scene.setParent(idCam, this.idHitbox)
                Scene.setPosY(idCam, 0.025)
            }
        }
        // sound
        {
            this.soundWalk = new SoundEmitter("footstep_walk.wav", true)
            this.soundWalk.setParent(this.idHitbox)
            this.soundRun = new SoundEmitter("footstep_walk.wav", true)
            this.soundRun.setParent(this.idHitbox)
            this.soundJump = new SoundEmitter("jump.mp3", false)
            this.soundJump.setParent(this.idHitbox)
        }
        // test
        {
            this.idSphereShader = Asset.Shader.load("blob.vert", "blob.frag")
            this.idSphereMesh = Asset.Mesh.loadStatic("icosphere.m3d")
            for (let i = 0; i < 1 << 17; ++i) {
                this.idSphereInstances.push(
                    Scene.MeshInstance.create(
                        this.idSphereMesh,
                        this.idSphereShader
                    )
                )
                let x = Math.random() * 10 - 5
                let y = Math.random() * 10 - 5
                let z = Math.random() * 10 - 5
                Scene.setPos(this.idSphereInstances[i], [x, y, z])
                Scene.setScale(this.idSphereInstances[i], [0.3, 0.3, 0.3])
            }
        }
        // state graph
        {
            this.state = new StateGraph()

            const move = (dt: number, speed: number) => {
                const x = speed * Input.leftX()
                const z = speed * Input.leftY()
                let newVelY = this.velY - this.gravity * dt
                this.velY = Math.min(
                    this.velYMax,
                    Math.max(this.velYMin, newVelY)
                )
                const dir = [x, this.velY, z]
                Scene.Physics.collideNSlide(this.idHitbox, dir, dt, {
                    x: 1,
                    z: 1,
                    y: 1,
                })
            }

            const idle = new StateNode("idle", {
                onTick: (dt: number) => {
                    // still want to do custom gravity and collideNSlide
                    move(dt, 0)
                },
            })
            const walk = new StateNode("walk", {
                onEnter: () => {
                    this.soundWalk?.start()
                },
                onExit: () => {
                    this.soundWalk?.stop()
                },
                onTick: (dt: number, time: number) => {
                    move(dt, this.walkSpeed)
                },
            })
            const run = new StateNode("run", {
                onEnter: () => {
                    this.soundRun?.start()
                },
                onExit: () => {
                    this.soundRun?.stop()
                },
                onTick: (dt: number, time: number) => {
                    move(dt, this.runSpeed)
                },
            })
            const jump = new StateNode("jump", {
                onEnter: (dt: number, time: number) => {
                    this.canJump = false
                    Scene.unsetParentKeepTransform(this.idHitbox)
                    this.soundJump?.start()

                    // don't apply gravity, it will be applied in onTick
                    let newVelY = this.velY + this.velYMax
                    this.velY = Math.min(
                        this.velYMax,
                        Math.max(this.velYMin, newVelY)
                    )
                },
                onTick: (dt: number, time: number) => {
                    move(dt, this.walkSpeed)
                },
            })

            idle.addTransition(jump, () => {
                return Input.getKey(Input.KEY_SPACE) || Input.buttonA()
            })
            idle.addTransition(run, () => {
                return (
                    (Input.buttonB() || Input.getKey(Input.KEY_LEFT_CONTROL)) &&
                    (Input.leftX() != 0 || Input.leftY() != 0)
                )
            })
            idle.addTransition(walk, () => {
                return Input.leftX() != 0 || Input.leftY() != 0
            })

            walk.addTransition(jump, () => {
                return Input.getKey(Input.KEY_SPACE) || Input.buttonA()
            })
            walk.addTransition(run, () => {
                return (
                    (Input.buttonB() || Input.getKey(Input.KEY_LEFT_CONTROL)) &&
                    (Input.leftX() != 0 || Input.leftY() != 0)
                )
            })
            walk.addTransition(idle, () => {
                return Input.leftX() == 0 && Input.leftY() == 0
            })

            run.addTransition(jump, () => {
                return Input.getKey(Input.KEY_SPACE) || Input.buttonA()
            })
            run.addTransition(walk, () => {
                return (
                    !(
                        Input.buttonB() || Input.getKey(Input.KEY_LEFT_CONTROL)
                    ) &&
                    (Input.leftX() != 0 || Input.leftY() != 0)
                )
            })
            run.addTransition(idle, () => {
                return Input.leftX() == 0 && Input.leftY() == 0
            })

            jump.addTransition(run, () => {
                return (
                    this.canJump &&
                    (Input.buttonB() || Input.getKey(Input.KEY_LEFT_CONTROL)) &&
                    (Input.leftX() != 0 || Input.leftY() != 0)
                )
            })
            jump.addTransition(walk, () => {
                return (
                    this.canJump && (Input.leftX() != 0 || Input.leftY() != 0)
                )
            })
            jump.addTransition(idle, () => {
                return this.canJump
            })

            this.state.addNode(idle)
            this.state.addNode(walk)
            this.state.addNode(jump)
            this.state.setCurrent(idle)
        }
    }
    __unload() {
        Scene.MeshInstance.destroyAll(this.idSphereInstances)
        this.idSphereInstances = []
        Asset.Mesh.destroy(this.idSphereMesh)
        Asset.Shader.destroy(this.idSphereShader)

        this.soundWalk?.destroy()
        this.soundRun?.destroy()
        this.soundJump?.destroy()

        Scene.Physics.destroy(this.idHitbox)
    }
    __main(dt: number, time: number) {
        // camera
        {
            this.camAngleY -= dt * 180 * Input.rightX()
            if (this.camAngleY < 0) this.camAngleY += 360
            if (this.camAngleY > 360) this.camAngleY -= 360
            Scene.setRotY(this.idHitbox, this.camAngleY)

            const newCamAngleX = this.camAngleX - dt * 90 * Input.rightY()
            if (newCamAngleX < 90 && newCamAngleX > -90)
                this.camAngleX = newCamAngleX
            Scene.setRotX(this.camera!.getId(), this.camAngleX)
        }
        // movement
        {
            this.state.tick(dt, time)
        }
        Scene.Entity.requestImGui(this.id)

        const idParent = Scene.getParent(this.idHitbox)
        if (idParent != 0) {
            // const posParent = Scene.getPosWorld(idParent)
            // const posThis = Scene.getPosWorld(this.idHitbox)
            // const newDelta = [posParent[0] - posThis[0], posParent[1] - posThis[1], posParent[2] - posThis[2]]
            // for(var i = 0; i < 3; i++)
            //     if(newDelta[i] != this.lastDelta[i])
            //     {
            //         printf(`${i} (${this.lastDelta[i]} => ${newDelta[i]}) ${dt}ms`)
            //     }
            // this.lastDelta = newDelta
            // printf(idParent, Scene.getUserPointer(idParent).vel)
        }
    }
    __collide_phorm(id: PhormID, normalWorld: number[]) {
        if (normalWorld[1] >= 0.9) {
            this.canJump = true
            this.velY = 0
            Scene.setParentKeepTransform(this.idHitbox, id)
        }
    }
    __no_collide() {
        // printf("No collisions")
    }
    __draw() {
        this.worldPos = Scene.getPos(this.idHitbox)
        Sound.setListenerWorldPos(this.worldPos)
        this.worldPos[1] += 1

        const dir = [
            -Math.sin((this.camAngleY * Math.PI) / 180),
            0,
            -Math.cos((this.camAngleY * Math.PI) / 180),
        ]
        Sound.setListenerWorldDir(dir)

        // update shaders AFTER CAMERA TRANSFORM IS DONE BEING MODIFIED
        Asset.Shader.setCameraUniforms(
            this.idSphereShader,
            this.camera!.getId()
        )
    }
    __imgui() {
        ImGui.text(
            `${this.worldPos[0]} ${this.worldPos[1]} ${this.worldPos[2]}`
        )
        const mag = ImGui.sliderFloat("Jump", this.velYMax, 0, 100)
        this.velYMax = mag
        this.velYMin = -mag
        this.gravity = ImGui.sliderFloat("Gravity", this.gravity, 0, 100)

        ImGui.separator()

        this.walkSpeed = ImGui.sliderFloat("Walk Speed", this.walkSpeed, 0, 25)
        this.runSpeed = ImGui.sliderFloat("Run Speed", this.runSpeed, 0, 25)
    }
}

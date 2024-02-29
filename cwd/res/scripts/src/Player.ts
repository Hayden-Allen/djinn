import "./lib/djinn.d"
import Entity from "./lib/Entity"
import Camera from "./lib/Camera"

const { Asset, Event, Scene, Input, ImGui, Sound } = djinn

export default class Player extends Entity {
    private camera?: Camera
    private idHitbox: PhysicsID
    private camAngleX: number = 0
    private camAngleY: number = 0
    private hitboxHeight: number = .1
    private hitboxRadius: number = .1
    private worldPos: number[] = [0, 0, 0]

    private isWalking: boolean = false
    private isRunning: boolean = false
    private canJump: boolean = false
    private velY: number = 0
    private velYMin: number = -100
    private velYMax: number = 20
    private gravity: number = 65
    private walkSpeed: number = 3
    private runSpeed: number = 5

    private idFootstep: SoundID = 0 as SoundID
    private idFootstepEmitter: SoundEmitterID = 0 as SoundEmitterID
    private idFootstepRun: SoundID = 0 as SoundID
    private idFootstepRunEmitter: SoundEmitterID = 0 as SoundEmitterID
    private idJump: SoundID = 0 as SoundID
    private idJumpEmitter: SoundEmitterID = 0 as SoundEmitterID

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
                Scene.setPosY(idCam, .025)
            }
        }

        this.idFootstep = Asset.Sound.load("footstep_walk.wav")
        this.idFootstepEmitter = Scene.SoundEmitter.create(this.idFootstep)
        Scene.SoundEmitter.setLooping(this.idFootstepEmitter, true)
        Scene.setParent(this.idFootstepEmitter, this.idHitbox)

        this.idFootstepRun = Asset.Sound.load("footstep_run.wav")
        this.idFootstepRunEmitter = Scene.SoundEmitter.create(this.idFootstepRun)
        Scene.SoundEmitter.setLooping(this.idFootstepRunEmitter, true)
        Scene.setParent(this.idFootstepRunEmitter, this.idHitbox)


        this.idJump = Asset.Sound.load("jump.mp3")
        this.idJumpEmitter = Scene.SoundEmitter.create(this.idJump)
        Scene.setParent(this.idJumpEmitter, this.idHitbox)
    }
    __unload() {
        Scene.SoundEmitter.destroy(this.idFootstepEmitter)
        Asset.Sound.destroy(this.idFootstep)

        Scene.SoundEmitter.destroy(this.idFootstepRunEmitter)
        Asset.Sound.destroy(this.idFootstepRun)

        Scene.SoundEmitter.destroy(this.idJumpEmitter)
        Asset.Sound.destroy(this.idJump)

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
            const boost = Input.buttonB() ? this.runSpeed : this.walkSpeed
            const x = boost * Input.leftX()
            const z = boost * Input.leftY()
            let newVelY = this.velY - this.gravity * dt
            if (this.canJump) {
                if (Input.buttonA()) {
                    this.canJump = false
                    newVelY += this.velYMax
                    Scene.unsetParentKeepTransform(this.idHitbox)
                    Scene.SoundEmitter.start(this.idJumpEmitter)
                }
            }
            this.velY = Math.min(
                this.velYMax,
                Math.max(this.velYMin, newVelY)
            )
            const dir = [x, this.velY, z]
            Scene.Physics.collideNSlide(this.idHitbox, dir, dt, { x: 1, z: 1, y: 1 })

            const moving = (x !== 0 || z !== 0) && this.canJump
            const walking = moving && !Input.buttonB()
            const running = moving && Input.buttonB()
            if(!this.isWalking && walking) {
                Scene.SoundEmitter.start(this.idFootstepEmitter)
            } else if(this.isWalking && !walking) {
                Scene.SoundEmitter.stop(this.idFootstepEmitter)
            }
            if(!this.isRunning && running) {
                Scene.SoundEmitter.start(this.idFootstepRunEmitter)
            } else if(this.isRunning && !running) {
                Scene.SoundEmitter.stop(this.idFootstepRunEmitter)
            }
            this.isWalking = walking
            this.isRunning = running
        }
        Scene.Entity.requestImGui(this.id)

        const idParent = Scene.getParent(this.idHitbox)
        if(idParent != 0)
        {
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
    }
    __imgui() {
        const mag = ImGui.sliderFloat("Jump", this.velYMax, 0, 100);
        this.velYMax = mag;
        this.velYMin = -mag;
        this.gravity = ImGui.sliderFloat("Gravity", this.gravity, 0, 100);

        ImGui.separator();

        this.walkSpeed = ImGui.sliderFloat("Walk Speed", this.walkSpeed, 0, 25);
        this.runSpeed = ImGui.sliderFloat("Run Speed", this.runSpeed, 0, 25);
    }
}

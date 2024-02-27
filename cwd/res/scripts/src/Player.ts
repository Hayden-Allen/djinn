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

    private canJump: boolean = false
    private velY: number = 0
    private velYMin: number = -100
    private velYMax: number = 20
    private gravity: number = 65
    private runSpeed: number = 1.5

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
    }
    __unload() {
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
            const boost = Input.getKey(Input.KEY_LEFT_CONTROL) ? 5 : 1
            const x = boost * this.runSpeed * Input.leftX()
            const z = boost * this.runSpeed * Input.leftY()
            let newVelY = this.velY - boost * this.gravity * dt
            if (this.canJump) {
                if (Input.getKey(Input.KEY_SPACE)) {
                    this.canJump = false
                    newVelY += this.velYMax
                    Scene.unsetParentKeepTransform(this.idHitbox)
                }
            }
            this.velY = Math.min(
                this.velYMax,
                Math.max(this.velYMin, newVelY)
            )
            const dir = [x, this.velY, z]
            Scene.Physics.collideNSlide(this.idHitbox, dir, dt, { x: 1, z: 1, y: 1 })
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

        this.runSpeed = ImGui.sliderFloat("Run Speed", this.runSpeed, 0, 25);
    }
}

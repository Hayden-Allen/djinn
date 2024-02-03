import "./service/id.d"

export default class Entity {
    protected id: EntityID = 0 as EntityID
    getId() {
        return this.id
    }
    __init(...args: any[]) {}
    __destroy() {}
    __load() {}
    __unload() {}
    __main(dt: number, time: number) {}
    __collide_entity(other: Entity, normalWorld: number[]) {}
    __collide_phorm(id: PhormID, normalWorld: number[]) {}
    __no_collide() {}
    __draw() {}
    __ui() {}
    __imgui() {}
}

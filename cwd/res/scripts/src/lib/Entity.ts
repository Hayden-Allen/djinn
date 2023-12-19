export default class Entity {
  protected id: number = -1
  getId() {
    return this.id
  }
  __init() {}
  __destroy() {}
  __load() {}
  __unload() {}
  __main(dt: number) {}
  __draw() {}
  __ui() {}
}

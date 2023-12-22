declare interface _IEntity {
  getId(): number
  __init(): void
  __destroy(): void
  __load(): void
  __unload(): void
  __main(dt: number): void
  __draw(): void
  __ui(): void
  __imgui(): void
}

type IEntity = _IEntity | undefined

import "./Entity.d"

declare interface _ICamera extends _IEntity {
  __load(): void
  __unload(): void
  __main(dt: number): void
}
type ICamera = _ICamera | undefined

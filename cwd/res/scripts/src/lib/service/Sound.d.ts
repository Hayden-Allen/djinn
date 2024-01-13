import "./id.d"

declare interface _Sound_Emitter {
  create(idSource: SoundID): SoundEmitterID
  play(id: SoundEmitterID): void
  stop(id: SoundEmitterID): void
  destroy(id: SoundEmitterID): void
}
export declare interface _Sound {
  Emitter: _Sound_Emitter
}

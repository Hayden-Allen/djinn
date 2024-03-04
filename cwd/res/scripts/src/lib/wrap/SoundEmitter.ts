import "../djinn.d"

const { Asset, Scene } = djinn

export default class SoundEmitter {
    private idSound: SoundID = 0 as SoundID
    private idEmitter: SoundEmitterID = 0 as SoundEmitterID

    constructor(fp: string, isLooping: boolean = false) {
        this.idSound = Asset.Sound.load(fp)
        this.idEmitter = Scene.SoundEmitter.create(this.idSound)
        Scene.SoundEmitter.setLooping(this.idEmitter, isLooping)
    }
    destroy() {
        Scene.SoundEmitter.destroy(this.idEmitter)
        Asset.Sound.destroy(this.idSound)
    }
    setParent(idParent: SceneID) {
        Scene.setParent(this.idEmitter, idParent)
    }
    start() {
        Scene.SoundEmitter.start(this.idEmitter)
    }
    stop() {
        Scene.SoundEmitter.stop(this.idEmitter)
    }
}

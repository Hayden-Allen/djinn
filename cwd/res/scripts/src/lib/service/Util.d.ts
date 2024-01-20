declare interface _Util {
    makeTexturePath(relPath: string): string
    listFiles(absolutePath: string): string[]
    vecConvertSpace(idFrom: SceneID, idTo: SceneID, vec: number[]): number[]
}

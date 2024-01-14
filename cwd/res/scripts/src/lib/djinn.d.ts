import type { _Asset } from "./service/Asset.d"
import "./service/ImGui.d"
import "./service/Input.d"
import "./service/Nanovg.d"
import "./service/Render.d"
import type { _Scene } from "./service/Scene.d"
import "./service/Util.d"

declare interface _Djinn {
    Asset: _Asset
    ImGui: _ImGui
    Input: _Input
    Nanovg: _Nanovg
    Render: _Render
    Scene: _Scene
    Util: _Util
}

declare global {
    const djinn: _Djinn
}

declare interface _ImGui {
    text(text: string): void
    colorPicker4(label: string, colors: number[]): number[]
    sliderFloat(label: string, val: number, min: number, max: number): number
    separator(): void
}

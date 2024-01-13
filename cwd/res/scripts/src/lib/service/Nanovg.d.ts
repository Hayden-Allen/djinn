declare interface _Nanovg {
  fillStyle(r: number, g: number, b: number, a?: number): void
  fillRect(x: number, y: number, w: number, h: number): void
  fillArc(
    x: number,
    y: number,
    r: number,
    start: number,
    end: number,
    dir: number
  ): void
  fillCircle(x: number, y: number, r: number): void
  strokeStyle(r: number, g: number, b: number, a?: number): void
  strokeLine(x0: number, y0: number, x1: number, y1: number): void
  strokeRect(x: number, y: number, w: number, h: number): void
  strokeArc(
    x: number,
    y: number,
    r: number,
    start: number,
    end: number,
    dir: number
  ): void
  strokeCircle(x: number, y: number, r: number): void
  setFont(name: string, size: number): void
  drawText(x: number, y: number, msg: string): void
}

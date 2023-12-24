export default class Color {
  public r: number = 0
  public g: number = 0
  public b: number = 0
  public a: number = 1

  constructor(r: number, g: number, b: number, a: number) {
    this.set(r, g, b, a)
  }
  set(r: number, g: number, b: number, a: number) {
    this.r = r
    this.g = g
    this.b = b
    this.a = a
  }
  toArray(): number[] {
    return [this.r, this.g, this.b, this.a]
  }
  fromArray(arr: number[]) {
    this.r = arr[0]
    this.g = arr[1]
    this.b = arr[2]
    this.a = arr[3]
  }
}

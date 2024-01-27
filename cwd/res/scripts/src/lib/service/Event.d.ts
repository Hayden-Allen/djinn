declare interface _Event {
    dispatch(event: string, payload: any): void
    subscribe(event: string, fn: Function, user?: any): void
    unsubscribe(event: string, fn: Function): void
}

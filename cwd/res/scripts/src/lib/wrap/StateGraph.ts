import "../djinn.d"

class StateTransition {
    public target?: StateNode
    public condition: Function = () => {}

    constructor(target: StateNode, condition: Function) {
        this.target = target
        this.condition = condition
    }
}

export class StateNode {
    public targets: StateTransition[] = []
    private name: string = ""
    private onEnter: Function = () => {}
    private onTick: Function = () => {}
    private onExit: Function = () => {}

    constructor(
        name: string,
        {
            onEnter = (dt: number, time: number) => {},
            onTick = (dt: number, time: number) => {},
            onExit = (dt: number, time: number) => {},
        } = {}
    ) {
        this.name = name
        this.onEnter = onEnter
        this.onTick = onTick
        this.onExit = onExit
    }
    addTransition(target: StateNode, condition: Function) {
        this.targets.push(new StateTransition(target, condition))
    }
    enter(dt: number, time: number) {
        this.onEnter(dt, time)
    }
    tick(dt: number, time: number) {
        this.onTick(dt, time)
    }
    exit(dt: number, time: number) {
        this.onExit(dt, time)
    }
}

export class StateGraph {
    private nodes: StateNode[] = []
    private current?: StateNode
    private newCurrent: boolean = false

    addNode(node: StateNode) {
        this.nodes.push(node)
    }
    setCurrent(node: StateNode) {
        this.current = node
        this.newCurrent = true
    }
    tick(dt: number, time: number) {
        if (this.newCurrent) {
            this.current?.enter(dt, time)
            this.newCurrent = false
        }

        for (const transition of this.current!.targets) {
            const target = transition.target
            if (transition.condition(dt, time)) {
                this.current?.exit(dt, time)
                this.current = target
                target?.enter(dt, time)
            }
        }

        this.current?.tick(dt, time)
    }
}

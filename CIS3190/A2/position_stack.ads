package Position_Stack is
    type Position;
    type Node;

    type NodePtr is access Node;
    type PositionPtr is access Position;

    type Position is record
        x: Integer;
        y: Integer;
        p: PositionPtr;
    end record;

    type Node is record
        pos: PositionPtr;
        next: NodePtr;
    end record;

    type PositionStack is record
        top: NodePtr;
        count: integer;
    end record;

    procedure push(s:in out PositionStack; pos: in PositionPtr);
    procedure pop(s:in out PositionStack; pos: out PositionPtr);
end Position_Stack;

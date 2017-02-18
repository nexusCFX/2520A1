with Gnat.Io; use Gnat.Io;

procedure mazeSolve is
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

    type Stack is record
        top: NodePtr;
    end record;

    procedure push(s:in out Stack; pos: in PositionPtr) is
        newNode: NodePtr;
    begin
        newNode := new Node'(pos, null);
        newNode.next := s.top;
        s.top := newNode;
    end push;

    procedure pop(s:in out Stack; pos: out PositionPtr) is
    begin
        pos := s.top.pos;
        s.top := s.top.next;
    end pop;

    NSStack: Stack;
    newPos: PositionPtr;
    Scan_Ptr: NodePtr;
    In_Int: Integer;
begin
    loop
        -- Read and integer, stopping at -1.
        Put("> ");
        Get(In_Int);
        exit when In_Int = -1;
        newPos = new Position'(In_int, 0, null);
        push(NSStack, newPos);

    end loop;

    -- Now print out the integers (backwards.)
    Scan_Ptr := NSStack.top;
    loop
        -- If the list is entirely empty, bail out now.
        exit when Scan_Ptr = null;

        -- Print, go to next.
        Put(Scan_Ptr.pos.x);
        Scan_Ptr := Scan_Ptr.next;

        -- If there is no next, we are done.
        exit when Scan_Ptr = null;

        -- Since there is a next, print a space to separate the items.
        Put(" ");
    end loop;

    New_Line;
end mazeSolve;

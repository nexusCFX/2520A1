with Gnat.Io; use Gnat.Io;

procedure mazeSolve is
    type Position;
    type Node;

    type NodePtr;
    type PositionPtr is access Position;

    type Position is record
        x: Integer;
        y: Integer;
        p: PositionPtr;
    end record;

    type Node is record
        pos: Position
        next: NodePtr
    end record;

    type Stack is record
        top: NodePtr;
    end record;

    procedure push(s: Stack; pos: in Position) is
        newNode: NodePtr;
    begin
        newNode := new Node'(pos, null);
        newNode.next := s.top;
        s.top := newNode;
    end push;

    procedure pop(s: Stack; pos: out Position) is
    begin
        out := s.top.pos;
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
        newPos = new Position'In_Int, 0, null);
        push(NSStack, newPos)

    end loop;

    -- Now print out the integers (backwards.)
    Scan_Ptr := Head;
    loop
        -- If the list is entirely empty, bail out now.
        exit when Scan_Ptr = null;

        -- Print, go to next.
        Put(Scan_Ptr.Data);
        Scan_Ptr := Scan_Ptr.Next;

        -- If there is no next, we are done.
        exit when Scan_Ptr = null;

        -- Since there is a next, print a space to separate the items.
        Put(" ");
    end loop;

    New_Line;
end mazeSolve;

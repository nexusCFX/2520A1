with Gnat.Io; use Gnat.Io;
procedure new is
    type Position;                           -- There is some type node.
    type PositionPtr is access Position;        -- Node_Ptr points to those.

    type Position is record                  -- Now I'll tell you what Node is.
        Data: Integer;
        Next: PositionPtr;
    end record;

    type Stack is record
        Top: PositionPtr;
    end record;

    stack: Stack;
    New_Node: PositionPtr;                  -- Created node.
    Scan_Ptr: PositionPtr;                  -- Scan the list.
    In_Int: Integer;                     -- Input integer.
begin
    loop
        -- Read and integer, stopping at -1.
        Put("> ");
        Get(In_Int);
        exit when In_Int = -1;

        New_Node := new Position'(In_Int, null);

        New_Node.Next := Stack.Top;
        Stack.Top := New_Node;

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
end new;

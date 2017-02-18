with Gnat.Io; use Gnat.Io;
procedure ll is
type Node;                           -- There is some type node.
type Node_Ptr is access Node;        -- Node_Ptr points to those.

type Node is record                  -- Now I'll tell you what Node is.
Data: Integer;
Next: Node_Ptr;
end record;

Head: Node_Ptr;                      -- Head of the list.
New_Node: Node_Ptr;                  -- Created node.
Scan_Ptr: Node_Ptr;                  -- Scan the list.
In_Int: Integer;                     -- Input integer.
begin
-- Read in a list of integers to -1.
loop
-- Read and integer, stopping at -1.
Put("> ");
Get(In_Int);
exit when In_Int = -1;

-- Create a node for it, filled in.  The initial values on new are
-- optional, but useful to fill in the new space, much like a
-- constructor.
New_Node := new Node'(In_Int, null);

-- Link it into the list.  (Actually, we could have set Next to be head
-- when we created the node in the last stmt, but I thought it would
-- be clearer this way.)
New_Node.Next := Head;
Head := New_Node;

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
end ll;

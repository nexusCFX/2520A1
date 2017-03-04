-- position_stack.ads
-- Stack package for holding Position objects
-- Written by Brandon Chester
-- Created on Feb 12, 2017
package Position_Stack is
    type Position;
    type Node;

    type NodePtr is access Node;
    type PositionPtr is access Position;

    -- Position object holds x, y coordinates, and a reference to previous position.
    type Position is record
        x: Integer;
        y: Integer;
        previous: PositionPtr;
    end record;

    -- Node is simply a wrapper for a position in the stack's internal linked list.
    type Node is record
        pos: PositionPtr;
        next: NodePtr;
    end record;

    -- Stack has top variable and a count
    type PositionStack is record
        top: NodePtr;
        count: integer;
    end record;

    -- Stack only needs push and pop operations defined
    procedure push(s:in out PositionStack; pos: in PositionPtr);
    procedure pop(s:in out PositionStack; pos: out PositionPtr);
end Position_Stack;

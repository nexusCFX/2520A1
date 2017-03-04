-- position_stack.adb
-- Implementation for stack package
-- Written by Brandon Chester
-- Created on Feb 12, 2017
package body Position_Stack is

    -- Pushes a new PositionPtr onto the stack
    procedure push(s:in out PositionStack; pos: in PositionPtr) is
    newNode: NodePtr;
    begin
        newNode := new Node'(pos, null);
        newNode.next := s.top;
        s.top := newNode;
        s.count := s.count + 1;
    end push;

    -- Pops a PositionPtr off the stack
    procedure pop(s:in out PositionStack; pos: out PositionPtr) is
    begin
        pos := s.top.pos;
        s.top := s.top.next;
        s.count := s.count - 1;
    end pop;

end Position_Stack;

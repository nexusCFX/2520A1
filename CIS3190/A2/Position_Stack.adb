with Ada.Text_IO; use Ada.Text_IO;
package body Position_Stack is

    procedure push(s:in out Stack; pos: in PositionPtr) is
    newNode: NodePtr;
    begin
        newNode := new Node'(pos, null);
        newNode.next := s.top;
        s.top := newNode;
        s.count := s.count + 1;
    end push;

    procedure pop(s:in out Stack; pos: out PositionPtr) is
    begin
        pos := s.top.pos;
        s.top := s.top.next;
        s.count := s.count - 1;
    end pop;

end Position_Stack;

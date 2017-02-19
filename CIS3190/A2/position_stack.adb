package body Position_Stack is

    procedure push(s:in out PositionStack; pos: in PositionPtr) is
    newNode: NodePtr;
    begin
        newNode := new Node'(pos, null);
        newNode.next := s.top;
        s.top := newNode;
        s.count := s.count + 1;
    end push;

    procedure pop(s:in out PositionStack; pos: out PositionPtr) is
    begin
        pos := s.top.pos;
        s.top := s.top.next;
        s.count := s.count - 1;
    end pop;

end Position_Stack;

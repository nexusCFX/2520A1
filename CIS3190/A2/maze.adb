with Ada.Text_IO; use Ada.Text_IO;
with stack; use stack;

procedure stack_eg is
    p : position;
    op : position;
begin
    p.x := 5;
    p.y := 7;
    push(p);
    pop(op);

    put(op);
end stack_eg;

with Gnat.Io; use Gnat.Io;

procedure maze is
    type Position;
    -- Something that points to a Position
    type PositionPtr is access Position;

    -- Define what the Position actually is
    type Position is record
        x: Integer;
        y: Integer;
        p:PositionPtr;
    end record;

    stack: array(Integer range 0 .. 9) of Position;
    ptr: PositionPtr;
begin
    stack(0).x = 1;
    stack(0).x = 2;
    stack(1).x = 3;
    stack(1).x = 4;
    stack(2).x = 5;
    stack(2).x = 6;
    stack(3).x = 7;
    stack(3).x = 8;

    ptr := stack(3);
    loop
        exit when ptr = null;
        Put(ptr.x);
        Put(",");
        Put(ptr.y);
        New_Line;
        ptr := ptr.previous;
        exit when ptr = null;
    end loop;
end maze;










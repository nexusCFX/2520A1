with Gnat.Io; use Gnat.Io;
procedure Ada is
    type Position;

    -- Something that points to a Position
    type PositionPtr is access Position;

    -- Define what the Position actually is
    type Position is record
        x: Integer;
        y: Integer;
        previous:PositionPtr;
    end record;

    stack: array(Integer range 0 .. 9) of Position;
    ptr: PositionPtr;
begin
    stack(0) := new Position(1, 2, null);
    stack(1) := new Position(3, 4, n0);
    stack(2) := new Position(5, 6, n1);
    stack(3) := new Position(7, 8, n2);
    stack(4) := new Position(9, 10, n3);
    stack(5) := new Position(11, 12, n4);
    stack(6) := new Position(13, 14, n5);
    stack(7) := new Position(15, 16, n6);
    stack(8) := new Position(17, 18, n7);
    stack(9) := new Position(19, 20, n8);
    ptr := stack(9);
    loop
        exit when ptr = null;
        Put(ptr.x);
        Put(",");
        Put(ptr.y);
        New_Line;
        ptr := ptr.previous;
        exit when ptr = null;
    end loop;
end Maze;










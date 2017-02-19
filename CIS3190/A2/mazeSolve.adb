with ada.Text_IO; use Ada.Text_IO;
with ada.Integer_Text_IO; use Ada.Integer_Text_IO;

procedure mazeSolve is
    type Position;
    type Node;

    type NodePtr is access Node;
    type PositionPtr is access Position;

    type Position is record
        x: Integer;
        y: Integer;
        p: PositionPtr;
    end record;

    type Node is record
        pos: PositionPtr;
        next: NodePtr;
    end record;

    type Stack is record
        top: NodePtr;
    end record;

    procedure push(s:in out Stack; pos: in PositionPtr) is
        newNode: NodePtr;
    begin
        newNode := new Node'(pos, null);
        newNode.next := s.top;
        s.top := newNode;
    end push;

    procedure pop(s:in out Stack; pos: out PositionPtr) is
    begin
        pos := s.top.pos;
        s.top := s.top.next;
    end pop;

    maze: array(0..49, 0..49) of character;
    solvedMaze: array(0..49, 0..49) of character;
    inputChar: character;
    NSStack: Stack;
    newPos: PositionPtr;
    Scan_Ptr: PositionPtr;
    numberOfRows: integer;
    numberOfColumns: integer;
    infp:file_type;
begin
    open(infp,in_file,"maze.txt");
    get(infp,numberOfRows);
    get(infp,inputChar);
    get(infp,numberOfColumns);
    numberOfRows := numberOfRows - 1;
    numberOfColumns := numberOfColumns - 1;
    loop
        exit when end_of_file(infp);
        for row in 0..numberOfRows loop
            for column in 0..numberOfColumns loop
                get(infp,inputChar);
                maze(row, column) := inputChar;
                solvedMaze(row, column) := inputChar;
            end loop;
        end loop;
    end loop;
    close(infp);
    for row in 0..numberOfRows loop
        for column in 0..numberOfColumns loop
            Ada.Text_IO.Put(maze(row, column));
        end loop;
    end loop;
end mazeSolve;

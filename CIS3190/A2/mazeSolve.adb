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
        count: integer;
    end record;

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

    maze: array(0..49, 0..49) of character;
    solvedMaze: array(0..49, 0..49) of character;
    inputChar: character;
    NSStack: Stack;
    curr: PositionPtr;
    temp: PositionPtr;
    prev: PositionPtr;
    numberOfRows: integer;
    numberOfColumns: integer;
    infp:file_type;
begin
    NSStack.count := 0;
    open(infp,in_file,"maze.txt");
    get(infp,numberOfRows, 2);
    get(infp,inputChar);
    get(infp,numberOfColumns, 2);
    numberOfRows := numberOfRows - 1;
    numberOfColumns := numberOfColumns - 1;
    for row in 0..numberOfRows loop
        for column in 0..numberOfColumns loop
            get(infp,inputChar);
            if (inputChar = 'o') then
                curr := new Position'(column, row, null);
            end if;
            maze(column, row) := inputChar;
            solvedMaze(column, row) := inputChar;
        end loop;
    end loop;
    close(infp);
    for row in 0..numberOfRows loop
        for column in 0..numberOfColumns loop
            Ada.Text_IO.Put(maze(column, row));
        end loop;
        new_line;
    end loop;

    push(NSStack, curr);
    while (stack.count /= 0) loop
        pop(NSStack, curr);
        if (maze(curr.x, curr.y) = 'e') then
            put_line("Found the path!");
            stack.count := 0;
            prev := curr.p;
            while (prev /= null) loop
                solvedMaze(prev.x, prev.y) := '@';
                prev := prev.p;
            end loop;

            for row in 0..numberOfRows loop
                for column in 0..numberOfColumns loop
                    Ada.Text_IO.Put(solvedMaze(column, row));
                end loop;
                new_line;
            end loop;
        elsif(maze(curr.x, curr.y) /= '*') then
            temp := new Position'(curr.x, curr.y + 1, curr);
            push(NSStack,temp);
            temp := new Position'(curr.x, curr.y - 1, curr);
            push(NSStack,temp);
            temp := new Position'(curr.x - 1, curr.y, curr);
            push(NSStack,temp);
            temp := new Position'(curr.x + 1, curr.y, curr);
            push(NSStack,temp);
        end if;
    end loop;

end mazeSolve;

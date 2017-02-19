with ada.Text_IO; use Ada.Text_IO;
with ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Position_Stack; use Position_Stack;

procedure mazeSolve is
    maze: array(0..49, 0..49) of character;
    solvedMaze: array(0..49, 0..49) of character;
    inputChar: character;
    stack: PositionStack;
    curr: PositionPtr;
    temp: PositionPtr;
    prev: PositionPtr;
    numberOfRows: integer;
    numberOfColumns: integer;
    fileName : string(1..100);
    fileNameLength : integer;
    infp:file_type;
begin
    stack.count := 0;
    put_line("Enter the name of the file containing your maze.");
    get_line(fileName, fileNameLength);
    open(infp,in_file,fileName);
    get(infp,numberOfRows);
    get(infp,inputChar);
    get(infp,numberOfColumns);
    numberOfRows := numberOfRows - 1;
    numberOfColumns := numberOfColumns - 1;
    for row in 0..numberOfRows loop
        for column in 0..numberOfColumns loop
            get(infp,inputChar);
            if (inputChar = 'o') then
                curr := new Position'(row, column, null);
            end if;
            maze(row, column) := inputChar;
            solvedMaze(row, column) := inputChar;
        end loop;
    end loop;
    close(infp);

    push(stack, curr);
    while (stack.count /= 0) loop
        pop(stack, curr);
        if (maze(curr.x, curr.y) = 'e') then
            stack.count := 0;
            prev := curr.p;
            while (prev.p /= null) loop
                solvedMaze(prev.x, prev.y) := '@';
                prev := prev.p;
            end loop;

            for row in 0..numberOfRows loop
                for column in 0..numberOfColumns loop
                    Ada.Text_IO.Put(solvedMaze(row, column));
                end loop;
                new_line;
            end loop;
        elsif(maze(curr.x, curr.y) /= '*') then
            maze(curr.x, curr.y) := '*';
            temp := new Position'(curr.x, curr.y + 1, curr);
            push(stack,temp);
            temp := new Position'(curr.x, curr.y - 1, curr);
            push(stack,temp);
            temp := new Position'(curr.x - 1, curr.y, curr);
            push(stack,temp);
            temp := new Position'(curr.x + 1, curr.y, curr);
            push(stack,temp);
        end if;
    end loop;
end mazeSolve;
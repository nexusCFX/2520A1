with ada.Text_IO; use Ada.Text_IO;
with ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Position_Stack; use Position_Stack;

procedure mazeSolve is
    type CharMaze is array(0..49, 0..49) of character;
    maze: CharMaze;
    solvedMaze: CharMaze;
    inputChar: character;
    stack: PositionStack;
    currentPosition: PositionPtr;
    newPosition: PositionPtr;
    tempPosition: PositionPtr;
    numberOfRows: integer;
    numberOfColumns: integer;
    fileName : string(1..100);
    fileNameLength : integer;
    infp:file_type;

procedure printMaze(maze: in CharMaze; rows: in integer; cols: in integer) is
begin
    for row in 0..rows loop
        for column in 0..cols loop
            put(maze(row, column));
            put(' ');
        end loop;
        new_line;
    end loop;
    new_line;
end printMaze;

begin
    stack.count := 0;
    put_line("Enter file name:");
    get_line(fileName, fileNameLength);
    open(infp,in_file,fileName);
    get(infp,numberOfColumns);
    get(infp,inputChar);
    get(infp,numberOfRows);

    numberOfRows := numberOfRows - 1;
    numberOfColumns := numberOfColumns - 1;
    for row in 0..numberOfRows loop
        for column in 0..numberOfColumns loop
            get(infp,inputChar);
            -- Note the position of the start when it's encountered
            if (inputChar = 'o') then
                currentPosition := new Position'(row, column, null);
            end if;

            -- Add chars to arrays, keep two copies
            maze(row, column) := inputChar;
            solvedMaze(row, column) := inputChar;
        end loop;
    end loop;
    close(infp);
    if (currentPosition = null) then
        put_line("Maze has no start. Exiting");
        return;
    end if;
    -- Output original unsolved maze
    put_line("Original maze:");
    printMaze(maze, numberOfRows, numberOfColumns);
    -- Maze solving portion using Stack
    push(stack, currentPosition);
    while (stack.count /= 0) loop
        pop(stack, currentPosition);
        if (maze(currentPosition.x, currentPosition.y) = 'e') then
            put_line("Maze traversed ok");
            put_line("End of maze found at location" & integer'image(currentPosition.x + 1)
                                                            & integer'image(currentPosition.y + 1));
            put_line("Path through maze (with dead ends):");
            printMaze(maze, numberOfRows, numberOfColumns);

            stack.count := 0;
            tempPosition := currentPosition.previous;
            while (tempPosition.previous /= null) loop
                solvedMaze(tempPosition.x, tempPosition.y) := 'v';
                tempPosition := tempPosition.previous;
            end loop;

            put_line("Path through maze (de-limbed):");
            printMaze(solvedMaze, numberOfRows, numberOfColumns);
            return;
        elsif(maze(currentPosition.x, currentPosition.y) /= '*' and maze(currentPosition.x, currentPosition.y) /= 'v') then
            maze(currentPosition.x, currentPosition.y) := 'v';
            newPosition := new Position'(currentPosition.x, currentPosition.y + 1, currentPosition);
            push(stack,newPosition);
            newPosition := new Position'(currentPosition.x, currentPosition.y - 1, currentPosition);
            push(stack,newPosition);
            newPosition := new Position'(currentPosition.x - 1, currentPosition.y, currentPosition);
            push(stack,newPosition);
            newPosition := new Position'(currentPosition.x + 1, currentPosition.y, currentPosition);
            push(stack,newPosition);
        end if;
    end loop;
    put_line("Maze is not connected! Exiting,");
end mazeSolve;

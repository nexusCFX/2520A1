with ada.Text_IO; use Ada.Text_IO;
with ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Position_Stack; use Position_Stack;

procedure mazeSolve is
    type CharMaze is array(0..49, 0..49) of character;
    maze: CharMaze;
    solvedMaze: CharMaze;
    stack: PositionStack;
    currentPosition: PositionPtr;
    newPosition: PositionPtr;
    tempPosition: PositionPtr;
    numberOfRows: integer;
    numberOfColumns: integer;
    fileReadSuccess: Boolean;

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

-- Get file and data within file
procedure readInput(maze, solvedMaze: out CharMaze; rows, cols: out integer; start: out PositionPtr; success: out Boolean) is
    inputChar: character;
    fileName : string(1..100);
    fileNameLength : integer;
    infp:file_type;
begin
    -- Read in initial info like file name. Get rows and columns.
    put_line("Enter file name:");
    get_line(fileName, fileNameLength);

    open(infp,in_file,fileName);
    get(infp,cols);
    get(infp,inputChar);
    get(infp,rows);

    rows := rows - 1;
    cols := cols - 1;
    for row in 0..rows loop
        for column in 0..cols loop
            get(infp,inputChar);
            -- Note the position of the start when it's encountered
            if (inputChar = 'o') then
                start := new Position'(row, column, null);
            end if;

            -- Add chars to arrays, keep two copies
            maze(row, column) := inputChar;
            solvedMaze(row, column) := inputChar;
        end loop;
    end loop;
    close(infp);
    success := true;
-- Handle missing files, bad file data
exception
    when name_error =>
        put_line("File does not exist. Exiting.");
        success := false;
    when data_error =>
        put_line("File data is not in expected format. Exiting.");
        success := false;

end readInput;

begin
    stack.count := 0;

    readInput(maze, solvedMaze, numberOfRows, numberOfColumns, currentPosition, fileReadSuccess);
    if (fileReadSuccess = false) then
        return;
    end if;

    -- Check currentPosition. If null, no 'o' was found, maze is invalid.
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

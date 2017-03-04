-- mazesolve.adb
-- Maze solving program written by Brandon Chester
-- Created on Feb 7, 2017

with ada.Text_IO; use Ada.Text_IO;
with ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Position_Stack; use Position_Stack;

-- Core program
procedure mazeSolve is

    -- Maze is a max of 50 x 50. Using C-style indices.
    type CharMaze is array(0..49, 0..49) of character;

    -- Make two mazes. solvedMaze will remove dead-end paths.
    maze: CharMaze;
    solvedMaze: CharMaze;

    -- See position_stack.ads and position_stack.adb for details.
    stack: PositionStack;

    -- Variables for tracking and traversing over positions
    currentPosition, newPosition, tempPosition: PositionPtr;

    numberOfRows, numberOfColumns: integer;
    fileReadSuccess: Boolean;

-- This procedure takes a CharMaze, its number of rows, and number of columns, and outputs it to console.
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

-- This procedure opens the input file and reads the data within the file. Also handles error checking.
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

    -- Decrement to match the c-style indices used in CharMaze.
    rows := rows - 1;
    cols := cols - 1;

    -- Read in all the characters in the file
    for row in 0..rows loop
        for column in 0..cols loop
            get(infp,inputChar);

            -- Note the position of the start when it's encountered
            if (inputChar = 'o') then
                start := new Position'(row, column, null);
            end if;

            -- Add chars to CharMazes, keep two copies
            maze(row, column) := inputChar;
            solvedMaze(row, column) := inputChar;
        end loop;
    end loop;
    close(infp);
    success := true;

-- Handle missing files, bad file data. Will set success to false, indicating the need to terminate execution in main program.
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

    -- Get the input data. End execution if an error occurs at that stage.
    readInput(maze, solvedMaze, numberOfRows, numberOfColumns, currentPosition, fileReadSuccess);
    if (fileReadSuccess = false) then
        return;
    end if;

    -- Check currentPosition. If null, no 'o' was found, maze is invalid. End execution.
    if (currentPosition = null) then
        put_line("Maze has no start. Exiting");
        return;
    end if;

    -- Output original unsolved maze
    put_line("Original maze:");
    printMaze(maze, numberOfRows, numberOfColumns);

    -- Maze solving portion using Stack to do DFS
    push(stack, currentPosition);
    while (stack.count /= 0) loop
        pop(stack, currentPosition);

        -- If we've found the end character, print solved maze and exit program.
        if (maze(currentPosition.x, currentPosition.y) = 'e') then
            put_line("Maze traversed ok");
            put_line("End of maze found at location" & integer'image(currentPosition.x + 1)
                                                     & integer'image(currentPosition.y + 1));

            -- Print the maze with solution and also dead ends.
            put_line("Path through maze (with dead ends):");
            printMaze(maze, numberOfRows, numberOfColumns);

            -- Go back through the chain of previous positions to find the true path, mark on solvedMaze.
            tempPosition := currentPosition.previous;
            while (tempPosition.previous /= null) loop
                solvedMaze(tempPosition.x, tempPosition.y) := 'v';
                tempPosition := tempPosition.previous;
            end loop;

            -- Print maze with only the true path marked.
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

    -- If we ever get here, the program returned to the start without finding a solution. Maze must not be solvable.
    put_line("Maze is not connected! Exiting.");
end mazeSolve;

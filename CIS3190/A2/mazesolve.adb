with ada.Text_IO; use Ada.Text_IO;
with ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Position_Stack; use Position_Stack;

procedure mazeSolve is
    maze: array(0..49, 0..49) of character;
    solvedMaze: array(0..49, 0..49) of character;
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
begin
    stack.count := 0;
    put_line("Enter file name:");
    get_line(fileName, fileNameLength);
    open(infp,in_file,fileName);
    get(infp,numberOfRows);
    get(infp,inputChar);
    get(infp,numberOfColumns);

    numberOfRows := numberOfRows - 1;
    numberOfColumns := numberOfColumns - 1;
    Ada.Text_IO.Put("Original maze:");
    new_line;
    for row in 0..numberOfRows loop
        for column in 0..numberOfColumns loop
            get(infp,inputChar);
            -- Output the original maze as it is read
            Ada.Text_IO.Put(inputChar);
            Ada.Text_IO.Put(' ');

            -- Note the position of the start when it's encountered
            if (inputChar = 'o') then
                currentPosition := new Position'(row, column, null);
            end if;

            -- Add chars to arrays, keep two copies
            maze(row, column) := inputChar;
            solvedMaze(row, column) := inputChar;
        end loop;
        new_line;
    end loop;
    new_line;
    close(infp);

    -- Maze solving portion using Stack
    push(stack, currentPosition);
    while (stack.count /= 0) loop
        pop(stack, currentPosition);
        if (maze(currentPosition.x, currentPosition.y) = 'e') then
            Ada.Text_IO.Put("Maze traversed ok");
            new_line;
            Ada.Text_IO.Put("End of maze found at location" & integer'image(currentPosition.x)
                                                            & integer'image(currentPosition.y));

            new_line;
            Ada.Text_IO.Put("Path through maze (with dead ends):");
            new_line;
            for row in 0..numberOfRows loop
                for column in 0..numberOfColumns loop
                    Ada.Text_IO.Put(maze(row, column));
                    Ada.Text_IO.Put(' ');
                end loop;
                new_line;
            end loop;
            new_line;

            stack.count := 0;
            tempPosition := currentPosition.previous;
            while (tempPosition.previous /= null) loop
                solvedMaze(tempPosition.x, tempPosition.y) := 'v';
                tempPosition := tempPosition.previous;
            end loop;

            Ada.Text_IO.Put("Path through maze (de-limbed):");
            new_line;
            for row in 0..numberOfRows loop
                for column in 0..numberOfColumns loop
                    Ada.Text_IO.Put(solvedMaze(row, column));
                    Ada.Text_IO.Put(' ');
                end loop;
                new_line;
            end loop;
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
end mazeSolve;

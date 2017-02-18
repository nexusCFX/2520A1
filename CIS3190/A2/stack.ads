package stack is
    type position is
        record
            x: Integer;
            y: Integer;
        end record;

    procedure push(x : in record);
    procedure pop( x : out record);
    function stack_is_empty return Boolean;
    function stack_top return record;
    procedure reset_stack;
end stack;

with Ada.Text_IO; use Ada.Text_IO;
package body stack is
    type list is array(0..2549) of position;
    type stack is
        record
            item : list;
            top : natural := 0;
        end record;

    st : stack;

    procedure push(x : in character) is
    begin
        if s.top = 100 then
            put_line(“stack is full”);
        else
            st.top = st.top +1;
            st.item(st.top) := x;
        end if;
    end push;

    procedure pop( x : out character) is
    begin
        if s.top = 0 then
            put_line(“stack is empty”);
        else
            st.top = st.top - 1;
        end if;
    end pop;

    function stack_is_empty return Boolean is
    begin
        return st.top = 0;
    end stack_is_empty;

    function stack_top return character is
    begin
        if st.top = 0 then
            put_line(“stack is empty”);
            return ‘‘;
        else
            return st.item(st.top);
        end if;
    end stack_top;

    procedure reset_stack is
    begin
        st.top := 0;
    end reset_stack;
end stack;

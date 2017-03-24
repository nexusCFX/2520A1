-- mazesolve.adb
-- Maze solving program written by Brandon Chester
-- Created on Feb 7, 2017

with ada.Text_IO; use Ada.Text_IO;
with ada.Integer_Text_IO; use Ada.Integer_Text_IO;

-- Core program
procedure mazeSolve is

    type SieveArray is array(Integer range <>) of Boolean;
    limit, a, i, j, k, l, sqrtOfLimit, quadratic: integer;

begin
    put_line("Enter the limit of the prime numbers you wish to find.");
    get(limit);

    declare
        sieve : SieveArray(0..limit);
    begin
        sieve(0) := false;
        sieve(1) := false;
        sieve(2) := true;
        sieve(3) := true;

        for i in 0..sqrtOfLimit loop
            for j in 0..sqrtOfLimit loop
                quadratic := (4*i*i) + (j*j);
                if ((quadratic rem 12 = 1 or quadratic rem 12 = 5) and quadratic <= limit) then
                    if (sieve(quadratic) = false) then
                        sieve(quadratic) := true;
                    else
                        sieve(quadratic) := false;
                    end if;
                end if;

                quadratic := (3*i*i) + (j*j);
                if (quadratic rem 12 = 7 and quadratic <= limit) then
                    if (sieve(quadratic) = false) then
                        sieve(quadratic) := true;
                    else
                        sieve(quadratic) := false;
                    end if;
                end if;

                quadratic := (3*i*i) - (j*j);
                if (quadratic rem 12 = 1 and i > j and quadratic <= limit) then
                    if (sieve(quadratic) = false) then
                        sieve(quadratic) := true;
                    else
                        sieve(quadratic) := false;
                    end if;
                end if;
            end loop;
        end loop;

        for k in 5..sqrtOfLimit loop
            if (sieve(k) = true) then
                a := k*k;
                for l in a..limit loop
                    sieve(l) := false;
                    l := l - 1 + a;
                end loop;
            end if;
        end loop;

        for i in 2..limit loop
            if (sieve(i) = true) then
                put_line(i);
            end if;
        end loop;
    end;
end mazeSolve;

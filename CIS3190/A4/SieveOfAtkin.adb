-- SieveOfAtkin.adb.
-- Adapted by Brandon Chester from Wikipedia algorithm for Sieve of Atkin
-- https://en.wikipedia.org/wiki/Sieve_of_Atkin
-- Created on 2017-03-23

with ada.Text_IO; use Ada.Text_IO;
with ada.Integer_Text_IO; use Ada.Integer_Text_IO;
with Ada.Numerics.Elementary_Functions; use Ada.Numerics.Elementary_Functions;
-- Core program
procedure SieveOfAtkin is

    type SieveArray is array(Integer range <>) of Boolean;
    limit, a, l, sqrtOfLimit, quadratic: integer;
    outFile:file_type;

begin
    put_line("Enter the limit of the prime numbers you wish to find.");
    get(limit);
    sqrtOfLimit := Integer(sqrt(Float(limit)));

    declare
        sieve : SieveArray(0..limit);
    begin
        for i in 0..limit loop
            sieve(i) := false;
        end loop;

        sieve(0) := false;
        sieve(1) := false;
        sieve(2) := true;
        sieve(3) := true;

        -- Wheel factorization
        for i in 0..sqrtOfLimit loop
            for j in 0..sqrtOfLimit loop
                quadratic := (4*i*i) + (j*j);
                -- Simplified form of Atkins' if quadratic mod 60 E {1,13,17,29,37,41,49,53}
                if ((quadratic rem 12 = 1 or quadratic rem 12 = 5) and quadratic <= limit) then
                    if (sieve(quadratic) = false) then
                        sieve(quadratic) := true;
                    else
                        sieve(quadratic) := false;
                    end if;
                end if;

                quadratic := (3*i*i) + (j*j);
                -- Simplified form of Atkins' if quadratic mod 60 E {7,19,31,43}
                if (quadratic rem 12 = 7 and quadratic <= limit) then
                    if (sieve(quadratic) = false) then
                        sieve(quadratic) := true;
                    else
                        sieve(quadratic) := false;
                    end if;
                end if;

                quadratic := (3*i*i) - (j*j);
                -- Simplified form of Atkins' if quadratic mod 60 E {11,23,47,59} and x > y
                if (quadratic rem 12 = 11 and i > j and quadratic <= limit) then
                    if (sieve(quadratic) = false) then
                        sieve(quadratic) := true;
                    else
                        sieve(quadratic) := false;
                    end if;
                end if;
            end loop;
        end loop;

        -- Sieve occurences marked as possibly prime on the wheel
        for k in 5..sqrtOfLimit loop
            if (sieve(k) = true) then
                a := k*k;
                l := a;
                while l <= limit loop
                    sieve(l) := false;
                    l := l + a;
                end loop;
            end if;
        end loop;

        create(outFile, out_file, "AdaPrimes.txt");
        put_line(outFile, "All primes up to " & integer'image(limit));
        -- Ignore 0 and 1 since primes are natural numbers > 1
        for i in 2..limit loop
            if (sieve(i) = true) then
                put_line(outFile, integer'image(i));
            end if;
        end loop;
    end;
end SieveOfAtkin;

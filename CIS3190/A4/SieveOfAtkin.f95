        ! SieveOfAtkin.f95.
        ! Adapted by Brandon Chester from Wikipedia algorithm for Sieve of Atkin
        ! https://en.wikipedia.org/wiki/Sieve_of_Atkin
        ! Created on 2017-03-23

        ! Main program
        program SieveOfAtkin
        implicit none
        integer :: limit, a, i, j, k, l, sqrtOfLimit, quadratic
        logical, allocatable, dimension(:) :: sieve
        real :: start, finish

        write(*,*) "Enter the limit of the prime numbers you wish to find."
        read (*,*) limit

        ! CPU time measurement
        call cpu_time(start)
        allocate(sieve(0:limit+1))

        sieve(0) = .false.
        sieve(1) = .false.
        sieve(2) = .true.
        sieve(3) = .true.

        sqrtOfLimit = int(sqrt(real(limit)))

        ! Wheel factorization
        do i = 0,sqrtOfLimit
            do j = 0,sqrtOfLimit
                quadratic = (4*i*i) + (j*j)
                ! Simplified form of Atkins' if quadratic mod 60 E {1,13,17,29,37,41,49,53}
                if ((mod(quadratic,12) == 1 .or. mod(quadratic,12) == 5) .and. quadratic <= limit) then
                    if (sieve(quadratic) .eqv. .false.) then
                        sieve(quadratic) = .true.
                    else
                        sieve(quadratic) = .false.
                    end if
                end if
                ! Simplified form of Atkins' if quadratic mod 60 E {7,19,31,43}
                quadratic = (3*i*i) + (j*j)
                if (mod(quadratic,12) == 7 .and. quadratic <= limit) then
                    if (sieve(quadratic) .eqv. .false.) then
                        sieve(quadratic) = .true.
                    else
                        sieve(quadratic) = .false.
                    end if
                end if

                quadratic = (3*i*i) - (j*j)
                ! Simplified form of Atkins' if quadratic mod 60 E {11,23,47,59} and x > y
                if (mod(quadratic,12) == 11 .and. i > j .and. quadratic <= limit) then
                    if (sieve(quadratic) .eqv. .false.) then
                        sieve(quadratic) = .true.
                    else
                        sieve(quadratic) = .false.
                    end if
                end if
            end do
        end do

        ! Sieve occurences marked as possibly prime on the wheel
        do k = 5,sqrtOfLimit
            if (sieve(k)) then
                a = k*k
                do l = a,limit,a
                    sieve(l) = .false.
                end do
            end if
        end do

        ! CPU time measurement
        call cpu_time(finish)
        write(*,199) finish-start
199     format("CPU execution time: ",f8.6," seconds")

        open(unit = 1,file = "FortranPrimes.txt",form = "formatted",status = "replace",action = "write")

        write(1,200) limit
200     format("All primes up to ",I10)
        ! Ignore 0 and 1 since primes are natural numbers > 1
        do i = 2,limit
            if (sieve(i) .eqv. .true.) then
                write(1,*) i
            end if
        end do

        close(unit = 1)
        write(*,*) "Your primes have been deposited in the file FortranPrimes.txt"

        end program SieveOfAtkin

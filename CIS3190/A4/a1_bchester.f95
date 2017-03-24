        ! Dice.f95.
        ! Adapted by Brandon Chester from game of dice written by Dr. Michael Wirth
        ! Created on 2017-01-12

        ! Main program
        program SieveOfAtkin
        implicit none
        integer :: limit, a, i, j, k, l, m, sqrtOfLimit, quadratic
        logical, allocatable, dimension(:) :: sieve

        write(*,*) "Enter the limit of the prime numbers you wish to find."
        read (*,*) limit

        allocate(sieve(0:limit+1))

        sieve(0) = .false.
        sieve(1) = .false.
        sieve(2) = .true.
        sieve(3) = .true.

        sqrtOfLimit = int(sqrt(real(limit)))

        do i = 0,sqrtOfLimit
            do j = 0,sqrtOfLimit
                quadratic = (4*i*i) + (j*j)
                if ((mod(quadratic,12) == 1 .or. mod(quadratic,12) == 5) .and. quadratic <= limit) then
                    if (sieve(quadratic) .eqv. .false.) then
                        sieve(quadratic) = .true.
                    else
                        sieve(quadratic) = .false.
                    end if
                end if

                quadratic = (3*i*i) + (j*j)
                if (mod(quadratic,12) == 7 .and. quadratic <= limit) then
                    if (sieve(quadratic) .eqv. .false.) then
                        sieve(quadratic) = .true.
                    else
                        sieve(quadratic) = .false.
                    end if
                end if

                quadratic = (3*i*i) - (j*j)
                if (mod(quadratic,12) == 1 .and. i > j .and. quadratic <= limit) then
                    if (sieve(quadratic) .eqv. .false.) then
                        sieve(quadratic) = .true.
                    else
                        sieve(quadratic) = .false.
                    end if
                end if
            end do
        end do

        do k = 5,sqrtOfLimit
            if (sieve(k)) then
                a = k*k
                do l = a,limit,a
                    sieve(l) = .false.
                end do
            end if
        end do

        do i = 2,limit
            if (sieve(i) .eqv. .true.) then
                write(*,*) i
            end if
        end do

        end program SieveOfAtkin

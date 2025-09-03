module rsi_module
    use iso_fortran_env, only : real64
    implicit none
    private

    public rsi

    contains

    subroutine rsi(rsis, vals, period)
        real(real64), dimension(:), intent(in out) :: rsis
        real(real64), dimension(:), intent(in)     :: vals
        integer,                    intent(in)     :: period

        real(real64) :: pos, neg, diff, temp
        integer      :: bars, per1, bar

        bars = size(rsis)
        per1 = period - 1
        pos  = 0.0_real64
        neg  = 0.0_real64

        rsis(1) = 0.0_real64

        do bar = 2, bars
            diff = vals(bar) - vals(bar - 1)

            ! Pos
            temp = pos * per1
            if (diff > 0.0_real64) then
                temp = temp + diff
            end if
            pos = temp / period

            ! Neg
            temp = neg * per1
            if (diff < 0.0_real64) then
                temp = temp - diff
            end if
            neg = temp / period

            rsis(bar) = 100.0_real64 - 100.0_real64 / (1.0_real64 + pos / neg)
        end do
    end subroutine rsi

end module rsi_module

module Rsi_module
    use iso_fortran_env, only : real64
    implicit none
    private

    public rsi

    contains

    subroutine rsi(rsi_ref, price, bars, period)
        real(real64), dimension(:), intent(inout) :: rsi_ref
        real(real64), dimension(:), intent(in)    :: price
        integer,                    intent(in)    :: bars
        integer,                    intent(in)    :: period

        real(real64) :: pos, neg, diff, temp
        integer      :: per1, bar

        per1 = period - 1
        pos  = 0.0_real64
        neg  = 0.0_real64

        rsi_ref(1) = 0.0_real64

        do bar = 2, bars
            diff = price(bar)
            diff = diff - price(bar - 1)

            ! Pos
            temp = pos
            temp = temp * per1
            if (diff > 0.0_real64) then
                temp = temp + diff
            end if
            temp = temp / period
            pos  = temp

            ! Neg
            temp = neg
            temp = temp * per1
            if (diff < 0.0_real64) then
                temp = temp - diff
            end if
            temp = temp / period
            neg  = temp

            rsi_ref(bar) = 100.0_real64 - 100.0_real64 / (1.0_real64 + pos / neg)
        end do
    end subroutine rsi

end module Rsi_module

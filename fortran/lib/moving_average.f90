module Moving_average_module
    use iso_fortran_env, only: real64
    implicit none
    private

    public simple_ma

    contains

    subroutine simple_ma(ma_ref, price, bars, period, shift)
        real(real64), dimension(:), intent(inout) :: ma_ref
        real(real64), dimension(:), intent(in)    :: price
        integer,                    intent(in)    :: bars
        integer,                    intent(in)    :: period
        integer,                    intent(in)    :: shift

        real(real64) :: temp, prev, sum
        integer      :: bar

        ma_ref(1:period + shift) = 0.0_real64

        sum = 0.0_real64
        do bar = 1, period
            sum = sum + price(bar)
        end do

        prev = sum / period
        ma_ref(period + shift) = prev

        do bar = period + 1, bars - shift
            temp = price(bar)
            temp = temp - price(bar - period)
            temp = temp / period
            prev = prev + temp
            ma_ref(bar + shift) = prev
        end do
    end subroutine simple_ma
end module Moving_average_module

module moving_average_module
    use iso_fortran_env, only: real64
    implicit none
    private

    public simple_ma

    contains

    subroutine simple_ma(ma, price, period, shift)
        real(real64), intent(in out) :: ma(:)
        real(real64), intent(in)     :: price(:)
        integer,      intent(in)     :: period, shift

        real(real64) :: psum, pinv
        integer      :: bar

        do bar = 1, period + shift - 1
            ma(bar) = 0.0_real64
        end do

        psum = 0.0_real64
        do bar = 1, period
            psum = psum + price(bar)
        end do

        pinv = 1.0_real64 / period
        ma(period + shift) = psum * pinv

        do bar = period + 1, size(ma) - shift
            ma(bar + shift) = ma(bar + shift - 1) + &
                (price(bar) - price(bar - period)) * pinv
        end do
    end subroutine simple_ma
end module moving_average_module

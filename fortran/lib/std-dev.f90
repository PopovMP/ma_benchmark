module std_dev_module
    use iso_fortran_env, only : real64
    implicit none
    private

    public std_dev

    contains

subroutine std_dev(devs, vals, mas, period)
    real(real64), intent(out) :: devs(:)
    real(real64), intent(in)  :: vals(:)
    real(real64), intent(in)  :: mas(:)
    integer,      intent(in)  :: period

    real(real64) :: sum, ma, d, p_inv
    integer      :: bar, n

    ! Input validation
    n = size(devs)
    if (period < 1 .or. period > size(vals) .or. n < size(mas) .or. size(vals) < size(mas)) then
        devs = 0.0_real64
        return
    end if

    ! Initialize output
    devs(1:min(period, n)) = 0.0_real64

    p_inv = 1.0_real64 / real(period, real64)

    ! Compute initial sum for the first valid window
    sum = 0.0_real64
    if (n > period) then
        ma = mas(period + 1)
        do bar = 1, period
            d = vals(period + 1 - bar) - ma
            sum = sum + d * d
        end do
        devs(period + 1) = sqrt(sum * p_inv)
    end if

    ! Slide the window for remaining bars
    do bar = period + 2, n
        ma = mas(bar)
        ! Remove oldest element
        d = vals(bar - period) - mas(bar - 1)
        sum = sum - d * d
        ! Add newest element
        d = vals(bar) - ma
        sum = sum + d * d
        ! Ensure sum doesn't go negative due to rounding
        if (sum < 0.0_real64) sum = 0.0_real64
        devs(bar) = sqrt(sum * p_inv)
    end do
end subroutine std_dev

end module std_dev_module

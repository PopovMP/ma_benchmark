program main
    use iso_fortran_env, only: real64
    use Data_set_module
    use Shared_state_module
    use Moving_average_module
    use Rsi_module
    use Std_dev_module
    implicit none

    character(256), parameter :: filepath       = "./data/EURUSD15.lb"
    integer,        parameter :: decimal_digits = 5
    type(Data_set_type)       :: data_set
    type(Shared_state_type)   :: shared_state
    integer                   :: bar

    data_set = read_data_set(filepath, decimal_digits)

    bar = data_set%bars
    print '("Bar: ", I0, ", ", I0, 4(", ", F7.5), 2(", ", I0))', &
             bar,                   &
             data_set%time   (bar), &
             data_set%open   (bar), &
             data_set%high   (bar), &
             data_set%low    (bar), &
             data_set%close  (bar), &
             data_set%volume (bar), &
             data_set%spreads(bar)

    call init_shared_state(shared_state, data_set%bars)

    call bench_ma     (shared_state, data_set, 10)
    call bench_ma     (shared_state, data_set, 10)
    call bench_rsi    (shared_state, data_set, 10)
    call bench_rsi    (shared_state, data_set, 10)
    call bench_std_dev(shared_state, data_set, 10)
    call bench_std_dev(shared_state, data_set, 10)

    contains

    subroutine bench_ma(state, data, runs)
        type(Shared_state_type), intent(in out) :: state
        type(Data_set_type),     intent(in)     :: data
        integer,                 intent(in)     :: runs

        integer      :: start, finish, count_rate
        real(real64) :: sum, elapsed_time

        call system_clock(count_rate=count_rate)
        call system_clock(count=start)

        sum = calc_ma(state, data, runs)

        call system_clock(count=finish)
        elapsed_time = real(finish - start) / real(count_rate)

        print '("MA    : ", F5.2, " seconds, sum: ", F10.5)', elapsed_time, sum
    end subroutine bench_ma

    subroutine bench_rsi(state, data, runs)
        type(Shared_state_type), intent(in out) :: state
        type(Data_set_type),     intent(in)     :: data
        integer,                 intent(in)     :: runs

        integer      :: start, finish, count_rate
        real(real64) :: sum, elapsed_time

        call system_clock(count_rate=count_rate)
        call system_clock(count=start)

        sum = calc_rsi(state, data, runs)

        call system_clock(count=finish)
        elapsed_time = real(finish - start) / real(count_rate)

        print '("RSI   : ", F5.2, " seconds, sum: ", F11.5)', elapsed_time, sum
    end subroutine bench_rsi

    subroutine bench_std_dev(state, data, runs)
        type(Shared_state_type), intent(in out) :: state
        type(Data_set_type),     intent(in)     :: data
        integer,                 intent(in)     :: runs

        integer      :: start, finish, count_rate
        real(real64) :: sum, elapsed_time

        call system_clock(count_rate=count_rate)
        call system_clock(count=start)

        sum = calc_std_dev(state, data, runs)

        call system_clock(count=finish)
        elapsed_time = real(finish - start) / real(count_rate)

        print '("StdDev: ", F5.2, " seconds, sum: ", F7.5)', elapsed_time, sum
    end subroutine bench_std_dev

    function calc_ma(state, data, runs) result(sum)
        type(Shared_state_type), intent(in out) :: state
        type(Data_set_type),     intent(in)     :: data
        integer,                 intent(in)     :: runs

        real(real64) :: sum
        integer      :: i, period, bars

        bars = data%bars
        sum  = 0.0_real64

        do i = 1, runs
            do period = 1, 199
                call simple_ma(state%numA, data%close, period, 0)
                sum = sum + state%numA(bars)
            end do
        end do
    end function calc_ma

    function calc_rsi(state, data, runs) result(sum)
        type(Shared_state_type), intent(in out) :: state
        type(Data_set_type),     intent(in)     :: data
        integer,                 intent(in)     :: runs

        real(real64) :: sum
        integer      :: i, period

        sum = 0.0_real64

        do i = 1, runs
            do period = 1, 199
                call rsi(state%numA, data%close, period)
                sum = sum + state%numA(data%bars)
            end do
        end do
    end function calc_rsi

    function calc_std_dev(state, data, runs) result(sum)
        type(Shared_state_type), intent(in out) :: state
        type(Data_set_type),     intent(in)     :: data
        integer,                 intent(in)     :: runs

        real(real64) :: sum
        integer      :: i, period

        sum = 0.0_real64

        do i = 1, runs
            do period = 1, 199
                call simple_ma(state%numA, data%close, period, 0)
                call std_dev  (state%numB, data%close, state%numA, period)
                sum = sum + state%numB(data%bars)
            end do
        end do
    end function calc_std_dev

end program main

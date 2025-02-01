program main
    use iso_fortran_env, only: real64
    use Data_set_module
    use Shared_state_module
    use Moving_average_module
    use Rsi_module
    implicit none

    character(256), parameter :: filepath       = "./data/EURUSD15.lb"
    integer,        parameter :: decimal_digits = 5
    type(Data_set_type)       :: data_set
    type(Shared_state_type)   :: shared_state
    integer                   :: bar

    data_set = read_data_set(filepath, decimal_digits)

    bar = data_set%bars
    print "(A,I0,A,I0,A,F7.5,A,F7.5,A,F7.5,A,F7.5,A,I0,A,I0)", &
             "Bar: ",                     &
             bar,                   ", ", &
             data_set%time   (bar), ", ", &
             data_set%open   (bar), ", ", &
             data_set%high   (bar), ", ", &
             data_set%low    (bar), ", ", &
             data_set%close  (bar), ", ", &
             data_set%volume (bar), ", ", &
             data_set%spreads(bar)

    call init_shared_state(shared_state, data_set%bars)

    call bench_ma(data_set, shared_state,  10)
    call bench_ma(data_set, shared_state,  10)
    call bench_rsi(data_set, shared_state,  10)
    call bench_rsi(data_set, shared_state,  10)

    contains

    subroutine bench_ma(data_set, shared_state, runs)
        type(Data_set_type),     intent(in)    :: data_set
        type(Shared_state_type), intent(inout) :: shared_state
        integer,                 intent(in)    :: runs

        integer      :: start, finish, count_rate
        real(real64) :: sum, elapsed_time

        call system_clock(count_rate=count_rate)
        call system_clock(count=start)

        sum = calc_ma(data_set, shared_state, runs)

        call system_clock(count=finish)
        elapsed_time = real(finish - start) / real(count_rate)

        print "(A,F5.2,A)", "MA time: ", elapsed_time, " seconds"
        print "(A,F12.5)",  "MA sum : ", sum
    end subroutine bench_ma

    subroutine bench_rsi(data_set, shared_state, runs)
        type(Data_set_type),     intent(in)    :: data_set
        type(Shared_state_type), intent(inout) :: shared_state
        integer,                 intent(in)    :: runs

        integer      :: start, finish, count_rate
        real(real64) :: sum, elapsed_time

        call system_clock(count_rate=count_rate)
        call system_clock(count=start)

        sum = calc_rsi(data_set, shared_state, runs)

        call system_clock(count=finish)
        elapsed_time = real(finish - start) / real(count_rate)

        print "(A,F5.2,A)", "RSI time: ", elapsed_time, " seconds"
        print "(A,F12.5)",  "RSI sum : ", sum
    end subroutine bench_rsi

    function calc_ma(data_set, shared_state, runs) result(sum)
        type(Data_set_type),     intent(in)    :: data_set
        type(Shared_state_type), intent(inout) :: shared_state
        integer,                 intent(in)    :: runs

        real(real64)                           :: sum
        real(real64), dimension(data_set%bars) :: closes, mas
        integer                                :: i, period, bars

        bars   = data_set%bars
        closes = data_set%close
        mas    = shared_state%numA
        sum    = 0.0_real64

        do i = 1, runs
            do period = 1, 199
                call simple_ma(mas, closes, bars, period, 0)
                sum = sum + mas(bars)
            end do
        end do
    end function calc_ma

    function calc_rsi(data_set, shared_state, runs) result(sum)
        type(Data_set_type),     intent(in)    :: data_set
        type(Shared_state_type), intent(inout) :: shared_state
        integer,                 intent(in)    :: runs

        real(real64)                           :: sum
        real(real64), dimension(data_set%bars) :: closes, rsis
        integer                                :: i, period, bars

        bars   = data_set%bars
        closes = data_set%close
        rsis   = shared_state%numA
        sum    = 0.0_real64

        do i = 1, runs
            do period = 1, 199
                call rsi(rsis, closes, bars, period)
                sum = sum + rsis(bars)
            end do
        end do
    end function calc_rsi
end program main

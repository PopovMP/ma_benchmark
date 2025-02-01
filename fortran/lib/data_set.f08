module Data_set_module
    use iso_fortran_env, only: real64, error_unit
    implicit none
    private

    type :: Data_set_type
        integer                                 :: bars = 0
        integer,      dimension(:), allocatable :: time
        real(real64), dimension(:), allocatable :: open
        real(real64), dimension(:), allocatable :: high
        real(real64), dimension(:), allocatable :: low
        real(real64), dimension(:), allocatable :: close
        integer,      dimension(:), allocatable :: volume
        integer,      dimension(:), allocatable :: spreads
    end type Data_set_type

    public Data_set_type
    public read_data_set, free_data_set

    integer, parameter :: record_width = 28 ! A bar data is encoded within 28 bytes

    contains

    function read_data_set(file_path, decimal_digits) result(data_set)
        character(len=256), intent(in) :: file_path
        integer,            intent(in) :: decimal_digits
        type(Data_set_type)            :: data_set

        integer :: bars

        bars = get_count_of_bars(file_path)
        if (bars == 0) return

        call allocate_data_set(data_set, bars)
        call read_bar_data(file_path, data_set, decimal_digits)
    end function read_data_set

    function get_count_of_bars(file_path) result(bars)
        character(len=256), intent(in) :: file_path
        integer                        :: bars

        integer            :: ios, file_size
        character(len=256) :: err_msg

        inquire(file=file_path, size=file_size, iostat=ios, iomsg=err_msg)

        if (ios == 0) then
            bars = file_size / record_width
        else
            bars = 0
            print '(A)', 'Error inquire file: "' // trim(file_path) // '" - ' // trim(err_msg)
        end if
    end function get_count_of_bars

    subroutine allocate_data_set(data_set, bars)
        type(Data_set_type), intent(inout) :: data_set
        integer,             intent(in)    :: bars

        data_set%bars = bars
        allocate(data_set%time   (bars))
        allocate(data_set%open   (bars))
        allocate(data_set%high   (bars))
        allocate(data_set%low    (bars))
        allocate(data_set%close  (bars))
        allocate(data_set%volume (bars))
        allocate(data_set%spreads(bars))
    end subroutine allocate_data_set

    subroutine read_bar_data(file_path, data_set, decimal_digits)
        character(len=256),  intent(in)    :: file_path
        type(Data_set_type), intent(inout) :: data_set
        integer,             intent(in)    :: decimal_digits

        integer, parameter          :: millenium   = 946677600 ! Seconds since 2000-01-01 00:00
        integer, parameter          :: int_pattern = 999999    ! Serves as pattern to `transfer`
        character(len=record_width) :: buffer
        character(len=256)          :: err_msg
        integer                     :: ios, file, i
        real(real64)                :: price_scale

        file        =  0
        price_scale = 10 ** decimal_digits

        open(unit=file, file=file_path, access='stream', status='old', iostat=ios, iomsg=err_msg)

        if (ios /= 0) then
            write(unit=error_unit, fmt='(A)') 'Error opening file: "' // trim(file_path) // '" - ' // trim(err_msg)
            stop 1
        end if

        do i = 1, data_set%bars
            read(unit=file, iostat=ios, iomsg=err_msg) buffer

            if (ios /= 0) then
                write(unit=error_unit, fmt='(A)') 'Error reading file: "' // trim(file_path) // '" - ' // trim(err_msg)
                stop 1
            end if

            data_set%time   (i) = 60 * transfer(buffer( 1: 4), int_pattern) + millenium
            data_set%open   (i) = real(transfer(buffer( 5: 8), int_pattern), real64) / price_scale
            data_set%high   (i) = real(transfer(buffer( 9:12), int_pattern), real64) / price_scale
            data_set%low    (i) = real(transfer(buffer(13:16), int_pattern), real64) / price_scale
            data_set%close  (i) = real(transfer(buffer(17:20), int_pattern), real64) / price_scale
            data_set%volume (i) =      transfer(buffer(21:24), int_pattern)
            data_set%spreads(i) =      transfer(buffer(25:28), int_pattern)
        end do

        close(file)
    end subroutine read_bar_data

    subroutine free_data_set(data_set)
        type(Data_set_type), intent(inout) :: data_set

        deallocate(data_set%time   )
        deallocate(data_set%open   )
        deallocate(data_set%high   )
        deallocate(data_set%low    )
        deallocate(data_set%close  )
        deallocate(data_set%volume )
        deallocate(data_set%spreads)
    end subroutine free_data_set

end module Data_set_module

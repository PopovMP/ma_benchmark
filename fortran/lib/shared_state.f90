module Shared_state_module
    use iso_fortran_env, only: real64
    implicit none
    private

    type, public :: Shared_state_type
        integer :: firstBar
        real(real64), dimension(:), allocatable :: numA
        real(real64), dimension(:), allocatable :: numB
        real(real64), dimension(:), allocatable :: numC
        real(real64), dimension(:), allocatable :: numD
        real(real64), dimension(:), allocatable :: balance
        real(real64), dimension(:), allocatable :: equity
    end type Shared_state_type

    public init_shared_state, deinit_shared_state

    contains

    subroutine init_shared_state(shared_state, bars)
        type(Shared_state_type), intent(inout) :: shared_state
        integer                                :: bars

        allocate(shared_state%numA   (bars))
        allocate(shared_state%numB   (bars))
        allocate(shared_state%numC   (bars))
        allocate(shared_state%numD   (bars))
        allocate(shared_state%balance(bars))
        allocate(shared_state%equity (bars))
    end subroutine init_shared_state

    subroutine deinit_shared_state(shared_state)
        type(Shared_state_type), intent(inout) :: shared_state

        deallocate(shared_state%numA   )
        deallocate(shared_state%numB   )
        deallocate(shared_state%numC   )
        deallocate(shared_state%numD   )
        deallocate(shared_state%balance)
        deallocate(shared_state%equity )
    end subroutine deinit_shared_state
end module Shared_state_module

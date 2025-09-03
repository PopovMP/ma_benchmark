module Shared_state_module
    use iso_fortran_env, only: real64
    implicit none
    private

    type :: Shared_state_type
        integer                                 :: firstBar
        real(real64), dimension(:), allocatable :: numA
        real(real64), dimension(:), allocatable :: numB
        real(real64), dimension(:), allocatable :: numC
        real(real64), dimension(:), allocatable :: numD
        real(real64), dimension(:), allocatable :: balance
        real(real64), dimension(:), allocatable :: equity
    end type Shared_state_type

    public Shared_state_type
    public init_shared_state, deinit_shared_state

    contains

    subroutine init_shared_state(state, bars)
        type(Shared_state_type), intent(inout) :: state
        integer,                 intent(in)    :: bars

        state%firstBar = 0
        allocate(state%numA   (bars), source=0.0_real64)
        allocate(state%numB   (bars), source=0.0_real64)
        allocate(state%numC   (bars), source=0.0_real64)
        allocate(state%numD   (bars), source=0.0_real64)
        allocate(state%balance(bars), source=0.0_real64)
        allocate(state%equity (bars), source=0.0_real64)
    end subroutine init_shared_state

    subroutine deinit_shared_state(state)
        type(Shared_state_type), intent(inout) :: state

        deallocate(state%numA   )
        deallocate(state%numB   )
        deallocate(state%numC   )
        deallocate(state%numD   )
        deallocate(state%balance)
        deallocate(state%equity )
    end subroutine deinit_shared_state
end module Shared_state_module

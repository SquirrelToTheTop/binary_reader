program main
  implicit none

  integer(kind=1) :: i4 = 2
  integer(kind=4) :: i, i32 = 174653, nvalues = 12
  real(kind=4) :: f32 = 3.1415
  real(kind=8) :: f64 = 1.6180339887498d0

  integer, dimension(4) :: ivals

  double precision, allocatable, dimension(:) :: dbvals
  double precision, parameter :: pi = 3.1415926535d0
  
  allocate( dbvals(nvalues) )

  do i=1, nvalues
    dbvals(i) = i*pi
  enddo

  do i=1, 4
    ivals(i) = ( i - 1 ) * 2 + 1
  enddo

  open(unit=42, file="test_f90.bin", form="unformatted", access="sequential")
    write(42) i4
    write(42) i32
    write(42) f32
    write(42) f64
    write(42) nvalues
    write(42) dbvals(:)
    write(42) 4
    write(42) ivals(:)
  close(42)

  deallocate( dbvals )

end program main

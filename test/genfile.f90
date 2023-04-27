program main
  implicit none

  integer(kind=1) :: i4 = 2
  integer(kind=4) :: i32 = 2
  real(kind=4) :: f32 = 3.1415
  real(kind=8) :: f64 = 1.6180339887498d0
  
  open(unit=42, file="test_f90.bin", form="unformatted", access="sequential")
    write(42) i4
    write(42) i32
    write(42) f32
    write(42) f64 
  close(42)

end program main

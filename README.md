# binary_reader

A very simple C++ code to read binary file produced by Fortran code (and eventually display record sizes). This is only one file `binary_reader.h`. See `main_test.cpp` for example of use.

Of course, the file format must be known. See `main_test.cpp` for interface
examples. A fortran binary can be produced by code in directory `test` (compilation: `gfortran genfile.f90` && execuation `a.out` which will generate a file named `test_f90.bin`.) The `main_test.cpp` is compiled with the `CMakeLists.txt` -> `mkdir build && cd build && cmake .. && make`, then `./F90Reader.exec test_f90.bin`.
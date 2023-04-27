#include <iostream>
#include "src/binary_reader.h"

int main( int argc, char* argv[] ){

  BinaryReaderF90 brf90 ( "test_f90.bin" );
  brf90.open();

  char c = brf90.accessVar<char>();
  int v = brf90.accessVar<int>();
  float f = brf90.accessVar<float>();
  double d = brf90.accessVar<double>();
  
  return EXIT_SUCCESS;
}
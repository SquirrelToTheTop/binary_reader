#include <iostream>
#include <vector>
#include "src/binary_reader.h"

int main( int argc, char* argv[] ){

  BinaryReaderF90 brf90 ( "test_f90.bin" );
  brf90.open();

  char c = brf90.accessVar<char>();
  int v = brf90.accessVar<int>();
  float f = brf90.accessVar<float>();
  double d = brf90.accessVar<double>();

  int nvals = brf90.accessVar<int>();
  
  std::vector<double> vd = brf90.accessVarArray_1d<double>( nvals );
  for( size_t i=0; i<vd.size(); ++i ){
    std::cout << "\t> vd[" << i << "] : " << vd[i] << std::endl;
  }

  int nvals_i = brf90.accessVar<int>();
  
  std::vector<int> id = brf90.accessVarArray_1d<int>( nvals_i );
  for( size_t i=0; i<id.size(); ++i ){
    std::cout << "\t> id[" << i << "] : " << id[i] << std::endl;
  }
  
  return EXIT_SUCCESS;
}
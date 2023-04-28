#include <iostream>
#include <vector>
#include "src/binary_reader.h"

int main( int argc, char* argv[] ){

  if( argc < 2 ) {
    std::cout << "\t> Missing binary file to read ..." << std::endl;

    std::string f = argv[0];
    return EXIT_FAILURE;
  }

  std::string fname = argv[1];

  {
    BinaryReaderF90 brf90 ( fname );
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

    brf90.close();

  }

  std::cout << "\n\t> Record analysis ... " << std::endl;
  {
    BinaryReaderF90 brf90 ( fname );
    brf90.open();
    brf90.analyzeRecord();
    brf90.close();
  }

  if( argc == 3 ){

  
    std::cout << "\n\t> Key - value access ... " << std::endl;
    {
      std::string fmtname = argv[2];

      BinaryReaderF90 brf90 ( fname );
      brf90.parseFormat( fmtname );
      brf90.buildMap();

      int nivals = brf90.read<int>( "nivals" );
      int nvalues = brf90.read<int>( "nvalues" );

      std::cout << "\t> 'nvalues' : " << nvalues << std::endl;
      std::cout << "\t> 'nivals' : " << nivals << std::endl;
    
    }
  
  }
  
  return EXIT_SUCCESS;
}
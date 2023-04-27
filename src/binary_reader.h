//
// Written by Squirrel
//

#pragma once

#include "../src/systemFileManip.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

#include <cassert>

#define BF90_VERBOSE

template<class a> inline constexpr std::string getVarName(){ assert(false); return "unknown_type"; };
template<> inline std::string getVarName<int>(){ return "int32_t"; }
template<> inline std::string getVarName<float>(){ return "float"; }
template<> inline std::string getVarName<double>(){ return "double"; }

class BinaryReaderF90 {
  
  private:
    std::ifstream _infile; // ( fname, std::ios::in | std::ios::binary );
    std::string _filename;

    // size of f90 record
    const int _f90_record_size = 4; 

  public:
    BinaryReaderF90( const std::string &fname ) : _filename( fname ){
      assert( !_filename.empty() );
      assert( checkFileExist( _filename ) );

    };

    ~BinaryReaderF90() = default;

    /**
     * Open file
     */
    bool open() {
      _infile.open( _filename, std::ios::in | std::ios::binary );
      return _infile.is_open();
    }

    /* Accessing single variable */
    template<typename var_t>
    inline
    var_t accessVar() {
      int record_in, record_out;

      // be sure of size of record match size of var for record
      assert( sizeof(int) == _f90_record_size );

      // initialize variable value
      var_t tmp = static_cast<var_t>( 0 );

      // read 1st fortran record
      _infile.read( reinterpret_cast<char *>( &record_in ), _f90_record_size );

#ifdef BF90_VERBOSE
      std::cout << "\n\t> Read : record [in] " << record_in << " bytes (" 
                << record_in / sizeof( var_t ) << " value)" << std::endl;
#endif

      // be sure there is only ONE value
      assert( record_in / sizeof( var_t ) == 1 );
      
      // be sure the type is correct (redundant with previous assert)
      assert( sizeof( tmp ) == record_in );

      // read variable data
      _infile.read( reinterpret_cast<char *>( &tmp ), sizeof( var_t ) );

#ifdef BF90_VERBOSE
      std::cout << "\t> Read : value : " << tmp << std::endl;
#endif

      // read 2nd fortran record
      _infile.read( reinterpret_cast<char *>( &record_out ), _f90_record_size );

#ifdef BF90_VERBOSE
      std::cout << "\t> Read : record [out] " << record_out << " bytes" << std::endl;
#endif

      assert( record_in == record_out );

      return tmp;
    }

    /**
     * Access a 1D array, this function performs a full read of the data 
     */
    template<typename var_t>
    inline
    std::vector<var_t> accessVarArray_1d( size_t nelem ) {
      int record_in, record_out;

      std::vector<var_t> tmp( nelem, static_cast<var_t>( 0 ) );

      // read 1st fortran record
      _infile.read( reinterpret_cast<char *>( &record_in ), _f90_record_size );

#ifdef BF90_VERBOSE
      std::cout << "\n\t> Read : record [in] " << record_in << " bytes (" 
                << record_in / sizeof(var_t) << " value)" << std::endl;
#endif

      // be sure there is only ONE value
      assert( record_in / sizeof( var_t ) == nelem );
      
      // read variable data
      _infile.read( reinterpret_cast<char *>( tmp.data() ), sizeof( var_t ) * nelem );

#ifdef BF90_VERBOSE
      std::cout << "\t> Read :  " << tmp.size() << " values ( " << sizeof( var_t ) * nelem << " bytes)" << std::endl;
#endif

      // read 2nd fortran record
      _infile.read( reinterpret_cast<char *>( &record_out ), _f90_record_size );

#ifdef BF90_VERBOSE
      std::cout << "\t> Read : record [out] " << record_out << " bytes" << std::endl;
#endif

      assert( record_in == record_out );

      // expect RVO
      return tmp;
    }

};
//
// Written by Squirrel
//

#pragma once

#include "../src/systemFileManip.h"

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <string_view>

#include <fstream>
#include <cstdint>
#include <cassert>

#include <unordered_map>

// #define BF90_VERBOSE

// template<typename type_t> constexpr std::string_view getName() { return "unknown"; }
// template<> constexpr std::string_view getName<char>()  { return "char";  }
// template<> constexpr std::string_view getName<int8_t>()  { return "int8_t";  }
// template<> constexpr std::string_view getName<uint8_t>() { return "uint8_t"; }
// template<> constexpr std::string_view getName<int16_t>() { return "int16_t"; }
// template<> constexpr std::string_view getName<int32_t>() { return "int32_t"; }
// template<> constexpr std::string_view getName<float>()   { return "float";   }
// template<> constexpr std::string_view getName<double>()  { return "double";  }

const std::unordered_map< std::string, int > sizeMap = { {"int8_t", 1}, {"uint32_t", 4},
{"int32_t", 4}, {"int64_t", 8}, {"float32_t", 4}, {"float64_t", 8},  };

struct InfoBlock {
  std::string m_name, m_type;
};

class BinaryReaderF90 {
  
  private:
    std::ifstream _infile;
    std::string _filename;

    // for direct access in key - value like
    std::vector<InfoBlock> fmtOrder;
    std::unordered_map<std::string, size_t> offsetMap;

    // size of f90 record in bytes
    const int _f90_record_size = 4; 

  public:

    BinaryReaderF90( const std::string &fname ) : _filename( fname ){
      assert( !_filename.empty() );
      assert( checkFileExist( _filename ) );

    };

    ~BinaryReaderF90(){
      if( _infile.is_open() ){
        _infile.close();
      }
    };

    /**
     * Open file
     */
    bool open() {
      _infile.open( _filename, std::ios::in | std::ios::binary );
      return _infile.is_open();
    }

    bool close() {
      _infile.close();
      return ~_infile.is_open();
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
                << record_in / sizeof( var_t ) << " value, type : " 
                << getName<var_t>() << ")" << std::endl;
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

    template<typename var_t>
    inline
    var_t read( const std::string & varName ) {
      int record_in, record_out;

      // be sure of size of record match size of var for record
      assert( sizeof(int) == _f90_record_size );

      assert( offsetMap.find( varName ) != offsetMap.end() );

      // initialize variable value
      var_t tmp = static_cast<var_t>( 0 );

      _infile.seekg( offsetMap.at( varName ) );

      // read 1st fortran record
      _infile.read( reinterpret_cast<char *>( &record_in ), _f90_record_size );

#ifdef BF90_VERBOSE
      std::cout << "\n\t> Read : record [in] " << record_in << " bytes (" 
                << record_in / sizeof( var_t ) << " value, type : " 
                << getName<var_t>() << ")" << std::endl;
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
                << record_in / sizeof(var_t) << " value, type : " 
                << getName<var_t>() << ")" << std::endl;
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

    /**
     * Access a 1D array, this function performs a full read of the data 
     */
    template<typename var_t>
    inline
    void accessVarArray_1d( std::vector<var_t> & buffer ) {
      assert( buffer.empty() );

      int record_in, record_out;

      // read 1st fortran record
      _infile.read( reinterpret_cast<char *>( &record_in ), _f90_record_size );

#ifdef BF90_VERBOSE
      std::cout << "\n\t> Read : record [in] " << record_in << " bytes (" 
                << record_in / sizeof( var_t ) << " value)" << std::endl;
#endif

      size_t nelem = record_in / sizeof( var_t ); 
      buffer( nelem, static_cast<var_t>( 0 ) );

      // read variable data
      _infile.read( reinterpret_cast<char *>( buffer.data() ), sizeof( var_t ) * nelem );

#ifdef BF90_VERBOSE
      std::cout << "\t> Read :  " << buffer.size() << " values ( " << sizeof( var_t ) * nelem << " bytes)" << std::endl;
#endif

      // read 2nd fortran record
      _infile.read( reinterpret_cast<char *>( &record_out ), _f90_record_size );

#ifdef BF90_VERBOSE
      std::cout << "\t> Read : record [out] " << record_out << " bytes" << std::endl;
#endif

      assert( record_in == record_out );

    }

    void analyzeRecord() {

      int record_in, record_out;

      _infile.seekg ( 0, _infile.end );
      size_t maxbytes = _infile.tellg();
      _infile.seekg (0, _infile.beg );
      
      std::cout << "\t\t> Max number of bytes : " << maxbytes << std::endl;

      bool keep_analyze = true;
      while( keep_analyze ){
        
        // read 1st record
        _infile.read( reinterpret_cast<char *>( &record_in ), _f90_record_size );

        // move cursor
        size_t cpos = _infile.tellg();
        _infile.seekg( cpos + record_in );

        // read 2nd record
        _infile.read( reinterpret_cast<char *>( &record_out ), _f90_record_size );

        assert( record_in == record_out );

        std::cout << "\t\t\t> Record of size " << record_in << " bytes" << std::endl;

        if( cpos >= maxbytes || _infile.rdstate() == std::ifstream::failbit 
            || _infile.rdstate() == std::ifstream::eofbit ){
          keep_analyze = false;
        }

      }

    }

    void parseFormat( const std::string &ff ){

      assert( checkFileExist( ff ) );

      std::ifstream in( ff, std::ios::in );
      assert( in.is_open() );
      
      std::string line;
      while ( std::getline(in, line) ){

        const std::string delimiter = ":";

        std::string v_id = line.substr(0, line.find( delimiter ) ); 
        std::string v_ty = line.substr(line.find( delimiter )+1, line.length() );

        fmtOrder.emplace_back( InfoBlock {v_id, v_ty} );

      }

      in.close();

    }

    void buildMap() {

      if( ! _infile.is_open() ) {
        open();
      }

      _infile.seekg ( 0, _infile.end );
      size_t maxbytes = _infile.tellg();
      _infile.seekg (0, _infile.beg );

      int record_in, record_out;
      bool keep_analyze = true;
      size_t datablock = 0;

      while( keep_analyze ){

        size_t offset_bf_rin = _infile.tellg();
        offsetMap.emplace( std::make_pair( fmtOrder[ datablock ].m_name, offset_bf_rin ) );
        
        // read 1st record
        _infile.read( reinterpret_cast<char *>( &record_in ), _f90_record_size );

        // move cursor
        size_t cpos = _infile.tellg();
        _infile.seekg( cpos + record_in );

        // read 2nd record
        _infile.read( reinterpret_cast<char *>( &record_out ), _f90_record_size );

        assert( record_in == record_out );

        datablock ++;

        if( cpos >= maxbytes || _infile.rdstate() == std::ifstream::failbit 
            || _infile.rdstate() == std::ifstream::eofbit || datablock >= fmtOrder.size() ){
          keep_analyze = false;
        }

      }
    }

};
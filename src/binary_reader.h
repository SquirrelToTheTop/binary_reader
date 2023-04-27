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
      int record_in = 0, record_out = 0;

      // be sure of size of record match size of var for record
      assert( sizeof(int) == _f90_record_size );

      // initialize variable value
      var_t tmp = static_cast<var_t>( 0 );

      // read 1st fortran record
      _infile.read( reinterpret_cast<char *>( &record_in ), _f90_record_size );

#ifdef BF90_VERBOSE
      std::cout << "\n\t> Read : record [in] " << record_in << " bytes (" 
                << record_in / sizeof(tmp) << " value)" << std::endl;
#endif

      // be sure there is only ONE value
      assert( record_in / sizeof( tmp ) == 1 );
      
      // be sure the type is correct (redundant with previous assert)
      assert( sizeof( tmp ) == record_in );

      // read variable data
      _infile.read( reinterpret_cast<char *>( &tmp ), sizeof( tmp ) );

#ifdef BF90_VERBOSE
      std::cout << "\t> Read : value : " << tmp << std::endl;
#endif

      // read 2nd fortran record
      _infile.read( reinterpret_cast<char *>( &record_out ), _f90_record_size );

#ifdef BF90_VERBOSE
      std::cout << "\t> Read : record [out] " << record_in << " bytes" << std::endl;
#endif

      assert( record_in == record_out );

      return tmp;
    }

    

};

//         /* Accessing 1D array variable with allocation of vector */
//         template<typename var_t>
//         inline
//         std::vector<var_t> _accessVarArray_1d( HIc_Obj &hicObj, const std::string &varName, size_t nelem ) const {
            
//             std::vector<var_t> tmp(nelem, static_cast<var_t>( 0 ) );
//             try{
//                 hicObj.getAttrVal( varName, tmp.data(), nelem );
//             } catch ( std::exception &_exp ){
//                 std::cerr << "\t> [accessVar] Failed for '" << varName << "'" << std::endl;
//                 std::cerr << "\t\t> Error: " << _exp.what() << std::endl;
//                 tmp.clear(); // -> make it empty  
//             }

//             return tmp;
//         }

//         /* Accessing 1D array variable filling pre-allocated vector */
//         template<typename var_t>
//         inline
//         void _accessVarArray_1d( HIc_Obj &hicObj, const std::string &varName, std::vector<var_t> &arr ) const {
//             assert( !arr.empty() );

//             // size_t nElemRead = 0;
//             try{
//                 hicObj.getAttrVal( varName, arr.data(), arr.size() ); // nElemRead );
//             } catch ( std::exception &_exp ){
//                 std::cerr << "\t> [accessVar] Failed for '" << varName << "'" << std::endl;
//                 std::cerr << "\t\t> Error: " << _exp.what() << std::endl;  
//             }
//         }

//         // Because Hercule bugs with long double (need reading long double array from binary file, named HKEY_XXXXX
//         // with XXXXX being the nstepcoarse
//         // File format must be:
//         // # 0: nkeys, key_0 ... key_n
//         void load_hkeys( Simulation_info &info, bool longKey ) const {

//             if( longKey ) {
//               if( _logs != nullptr ){
//                 *_logs << "[INFO][hercule_lightAMR_read.cpp]\t: Reading domains SFC boundaries as long double (128 bits)" << std::endl;
//               }
//             }else {
//               if( _logs != nullptr ){
//                 *_logs << "[INFO][hercule_lightAMR_read.cpp]\t: Reading domains SFC boundaries as double (64 bits)" << std::endl;
//               }
//             }
            
//             char fname[128];

//             // // format file name to hkeys_xxxxx
//             sprintf( &fname[0], "hkeys_%05d", info.getNumberOfCoarseStep() );
            
//             if( !checkFileExist( fname ) ){
//               std::cout << "\t> [Hercule_HDep_Reader]::load_hkeys cannot find file '" << fname << "'" << std::endl;
//             }

//             if( _logs != nullptr ){
//                 *_logs << "[INFO][hercule_lightAMR_read.cpp]\t: Reading domains SFC boundaries from '"
//                        << fname << "'" << std::endl;
//               }
//             std::ifstream infile( fname, std::ios::in | std::ios::binary );
            
//             if( ! infile.is_open() ) {
//                 std::cerr << "\t> [Hercule_HDep_Reader]::load_hkeys Error cannot open file !" << std::endl;
//                 return;
//             }

//             std::string tmp_str;
//             int tmp_val;
            
//             // first line is var_name (nstep_coarse) and its value
//             infile >> tmp_str >> tmp_val;
//             assert( tmp_val == info.getNumberOfCoarseStep() ); // be sure we read the correct coarse step !

//             // next line is number of keys, must be ndomains+1
//             infile >> tmp_str >> tmp_val;
//             assert( tmp_val == info.getNumberOfDomains() + 1 );         

//             if( longKey ){

//               std::cerr << "\t> [Hercule_HDep_Reader]::load_hkeys for now hkeys loaded as uint64_t ! " << std::endl;
//               std::vector<uint64_t> hk( info.getNumberOfDomains() + 1, 0 );
//               for( int i=0; i<=info.getNumberOfDomains(); ++i ){
//                 long double hki;
//                 infile >> hki;

//                 hk[ i ] = static_cast<uint64_t>( hki );
//                 std::cout << "\t> Key [ " << i << "] : " << hk[ i ] << std::endl;
//                 assert( hk[ i ] >= 0 && hk[ i ] < std::numeric_limits<uint64_t>::max() );
//               }
//               info.setDomainDecompositionBoundaries( hk );

//             }else{
//               std::vector<uint64_t> hk( info.getNumberOfDomains() + 1, 0 );
//               for( int i=0; i<=info.getNumberOfDomains(); ++i ){
//                 double hki;
//                 infile >> hki;

//                 hk[ i ] = static_cast<uint64_t>( hki );
//                 assert( hk[ i ] >= 0 && hk[ i ] < std::numeric_limits<uint64_t>::max() );
//                 // std::cout << "\t> Key [ " << i << "] : " << hk[ i ] << std::endl;
//               }
//               info.setDomainDecompositionBoundaries( hk );
//             }
          
//         }

//     public:
        
//         /* -------------- Base manipulation -------------- */ 
        
//         // Initialize object
//         Hercule_HDep_Reader( const std::string &base_path, bool verbosity );

//         // destructor
//         ~Hercule_HDep_Reader();

//         bool Update();

//         void setLogs( std::ofstream &logs ){ _logs = &logs; }

//         // close base 
//         void close_Base();

//         /**
//          * Open a hercule Hdep database at a given (domain, snapshot)
//          * @param domain
//          * @param snapshot
//          */
//         void open_Context(int domain, int snapshot);
//         void close_Context();

//         // load meta information of a context
//         Simulation_info load_meta( int snapshot, bool hkeyFix, bool longKey );

//         // load basics data for load balancing
//         void load_basics(LightAMR *, int, bool);

// #ifdef BUILD_PYTHON_API
//         // For python API (in order to avoid dealing with std::move & rvalue)
//         void load_amr_py( int domain, int snapshot, const Simulation_info &info, DataHydro &data ){
//             auto l = load_amr( domain, snapshot, info );
//             data.addMesh( domain, std::move( l ) );
//         }

//         void
//         load_quantity_py( int domain, int snapshot, const Simulation_info &info, const std::string &field, DataHydro &data ){
//             auto & la = data.getRefOnMesh( domain );
//             load_quantity( info, field, la );
//         }
// #endif
//         // load and return a lightAMR object for amr description
//         LightAMR load_amr(int domain, int snapshot, const Simulation_info &);

//         // load particles data
//         Particules load_particles(int);

//         std::vector<std::string> getStoredQuantityName(const Simulation_info &info);

//         /**
//          * load a scalar quantity
//          *
//          * @param info  : Simulation_info object
//          * @param field : string, field name to load
//          * @param mesh  : LightAMR object
//          */
//         void load_quantity(const Simulation_info &info, const std::string &field, LightAMR &mesh);

//         /* -------------- GETTER -------------- */
//         int getNumberOfDomains() const { return _ndomains; }
//         int getNumberOfTimes() const { return static_cast<int>(_all_times.size()); }
//         std::vector<double> getListOfTimes() const { return _all_times; };

// };

// #endif
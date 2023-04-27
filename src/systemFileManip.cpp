#include "systemFileManip.h"
#include <errno.h>

#include <iostream>

/**
 * Expand path given in parameter to allow the use of "~" in path.
 * @param in (in, std::string) : path starting with ~
 */
void expandPath(std::string &in){
    if( in[0] == '~' ){
        std::string home = getenv("HOME");
        in = home + in.substr(1, in.length());
    }
}

/**
 * Check if path refer to an existing directory
 *
 * @param in (in, std::string) : path to directory
 * @return
 */
bool checkDirExist(const std::string &in){
    bool exist = false;

    struct stat inf = {0};
    stat(in.c_str(), &inf);

    if( S_ISDIR(inf.st_mode) ){
        return true;
    }

    return exist;
}

/**
 * Check if path refer to an existing regular file
 *
 * @param in (in, std::string) : path to directory
 * @return
 */
bool checkFileExist(const std::string &in){
    bool exist = false;

    struct stat inf = {0};
    stat(in.c_str(), &inf);

    if( S_ISREG(inf.st_mode) ){
        return true;
    }

    return exist;
}

/**
 * Create directory if it does not exists already
 * @param out (in, std::string) : path to directory
 * @return
 */
bool createIfNotExist(const std::string &out) {
    int res = mkdir(out.c_str(), 0700);

    if (res == 0){
        return true;
    }else {
        if( errno == EEXIST ) {
            std::cerr << "\t> [createIfNotExist] Error cannot create dir '" << out << "', already exists ! "
                      << std::endl;
        }else {
            std::cout << "\t [createIfNotExist] Error cannot create dir, error: " << res << std::endl;
        }
    }

    return false;
}


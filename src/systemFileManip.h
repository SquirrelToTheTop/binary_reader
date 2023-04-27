#include <stdlib.h>
#include <sys/stat.h>
#include <string>

/**
 * Expand path given in parameter to allow the use of "~" in path.
 * @param in (in, std::string) : path starting with ~
 */
void expandPath(std::string &in);

/**
 * Check if directory exists.
 *
 * @param in (in, std::string) : expanded path
 * @return boolean true if exists
 */
bool checkDirExist(const std::string &in);

/**
 * Check if file exists.
 *
 * @param in (in, std::string) : expanded path
 * @return
 */
bool checkFileExist(const std::string &in);

/**
 * Attempt to create directory
 * @param out (in, std::string) : path to dir
 * @return
 */
bool createIfNotExist(const std::string &out);
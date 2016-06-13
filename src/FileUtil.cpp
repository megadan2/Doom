#include "FileUtil.h"
#include <io.h>

#define R_OK    4   // Read
#define W_OK    2   // Write
#define X_OK    1   // Execute (not supported on Windows)
#define F_OK    0   // Exists

namespace Doom {

//=============================================================================
bool FileUtil::HasReadAccess( const string& str ) {
    return !_access( str.c_str(), R_OK );
}

}
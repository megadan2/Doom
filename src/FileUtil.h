#pragma once

#include <string>
using std::string;

namespace Doom {

class FileUtil {
public:
    static bool HasReadAccess( const string& str );

};

}
#include "StringUtil.h"

namespace Doom {

//=============================================================================
bool StringUtil::EndsWith( const char* strA, const char* strB ) {
    int lenA = strlen( strA );
    int lenB = strlen( strB );

    return _strcmpi( strA + lenA - lenB, strB ) == 0 ? true : false;
}

}
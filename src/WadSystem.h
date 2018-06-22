#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace Doom {

//=============================================================================
struct LumpInfo {
    char        name[8];
    int         handle;
    int         position;
    int         size;
};

//=============================================================================
class WadSystem {
public:
                WadSystem();

    void        InitMultipleFiles( const vector<string>& files );
    int         CheckNumForName( const char* name );

private:
    void        AddFile( const char* filename );
    void        ExtractFileBase( const char* path, char* dest );

private:
    LumpInfo*   m_lumpInfo;
    int         m_numLumps;
    void**      m_lumpCache;
    int         m_reloadLump;
    const char* m_reloadName;
};

}

#include "WadSystem.h"

namespace Doom {

//=============================================================================
WadSystem::WadSystem() :
    m_numLumps( 0 )
{}

//=============================================================================
// All files are optional, but at least one file must be found. Files with a 
// .wad extension are idlink files with multiple lumps.  Other files are single 
// lumps with the base filename for the lump name. Lump names can appear 
// multiple times. The name searcher looks backwards, so a later file does 
// override all earlier ones.
//=============================================================================
void WadSystem::InitMultipleFiles( const vector<string>& files ) {

}

}
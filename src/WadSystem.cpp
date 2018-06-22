#include "WadSystem.h"
#include "StringUtil.h"
#include "Swap.h"
#include <io.h>
#include <fcntl.h>

namespace Doom {

//=============================================================================
struct WadInfo {
    char        identification[4]; // Should be "IWAD" or "PWAD"
    int         numLumps;
    int         infoTableOfs;
};

//=============================================================================
struct FileLump {
    int         filePos;
    int         size;
    char        name[8];
};

//=============================================================================
WadSystem::WadSystem() :
    m_numLumps( 0 ),
    m_lumpCache( nullptr ),
    m_reloadLump( 0 ),
    m_reloadName( nullptr )
{}

//=============================================================================
// All files are optional, but at least one file must be found. Files with a 
// .wad extension are idlink files with multiple lumps.  Other files are single 
// lumps with the base filename for the lump name. Lump names can appear 
// multiple times. The name searcher looks backwards, so a later file does 
// override all earlier ones.
//=============================================================================
void WadSystem::InitMultipleFiles( const vector<string>& files ) {
    // open all the files, load headers, and count lumps
    m_numLumps = 0;

    // will be realloced as lumps are added
    m_lumpInfo = (LumpInfo*)malloc( 1 );

    for ( auto& file : files ) {
        AddFile( file.c_str() );
    }

    if ( !m_numLumps ) {
        //TODO I_Error( "W_InitFiles: no files found" );
    }

    // set up caching
    int size = m_numLumps * sizeof( *m_lumpCache );
    m_lumpCache = (void**)malloc( size );

    if ( !m_lumpCache ) {
        //TODO I_Error( "Couldn't allocate lumpcache" );
    }

    memset( m_lumpCache, 0, size );
}

//=============================================================================
// Returns -1 if name not found.
//=============================================================================
int WadSystem::CheckNumForName( const char* name ) {
    // scan backwards so patch lump files take precedence
    LumpInfo* lump = m_lumpInfo + m_numLumps;
    while ( lump-- != m_lumpInfo ) {
        if ( _stricmp( lump->name, name ) == 0 ) {
            return lump - m_lumpInfo;
        }
    }

    // TFB. Not found.
    return -1;
}

//=============================================================================
// All files are optional, but at least one file must be found (PWAD, if all 
// required lumps are present). Files with a .wad extension are wadlink files
// with multiple lumps. Other files are single lumps with the base filename for 
// the lump name.
//
// If filename starts with a tilde, the file is handled specially to allow map 
// reloads. But: the reload feature is a fragile hack...
//=============================================================================
void WadSystem::AddFile( const char* filename ) {
    // open the file and add to directory

    // handle reload indicator.
    if ( filename[0] == '~' ) {
        filename++;
        m_reloadName = filename;
        m_reloadLump = m_numLumps;
    }

    int handle;
    if ( ( handle = _open( filename, O_RDONLY | O_BINARY ) ) == -1 ) {
        printf( " couldn't open %s\n", filename );
        return;
    }

    printf( " adding %s\n", filename );
    int startLump = m_numLumps;

    FileLump* fileInfo;
    FileLump singleInfo;

    if ( !StringUtil::EndsWith( filename, "wad" ) ) {
        // single lump file
        fileInfo = &singleInfo;
        singleInfo.filePos = 0;
        singleInfo.size = LONG( _filelength( handle ) );
        ExtractFileBase( filename, singleInfo.name );
        m_numLumps++;
    } else {
        // WAD file
        WadInfo header;
        _read( handle, &header, sizeof( header ) );
        if ( strncmp( header.identification, "IWAD", 4 ) ) {
            // Homebrew levels?
            if ( strncmp( header.identification, "PWAD", 4 ) ) {
                //TODO I_Error( "Wad file %s doesn't have IWAD or PWAD id\n", filename );
            }
            // ???modifiedgame = true;
        }
        header.numLumps = LONG( header.numLumps );
        header.infoTableOfs = LONG( header.infoTableOfs );
        int length = header.numLumps * sizeof( FileLump );
        fileInfo = new FileLump[header.numLumps];
        _lseek( handle, header.infoTableOfs, SEEK_SET );
        _read( handle, fileInfo, length );
        m_numLumps += header.numLumps;
    }

    // File in lumpinfo
    m_lumpInfo = (LumpInfo*)realloc( m_lumpInfo, m_numLumps * sizeof( LumpInfo ) );
    if ( !m_lumpInfo ) {
        //TODO I_Error ("Couldn't realloc lumpinfo");
    }

    LumpInfo* lump = &m_lumpInfo[startLump];
    int storeHandle = m_reloadName ? -1 : handle;

    for ( int i = startLump; i < m_numLumps; i++, lump++, fileInfo++ ) {
        lump->handle = storeHandle;
        lump->position = LONG( fileInfo->filePos );
        lump->size = LONG( fileInfo->size );
        strncpy( lump->name, fileInfo->name, 8 );
    }

    if ( m_reloadName ) {
        _close( handle );
    }
}

//=============================================================================
void WadSystem::ExtractFileBase( const char* path, char* dest ) {
    const char* src = path + strlen( path ) - 1;

    // back up until a \ or the start
    while ( src != path && *(src - 1) != '\\' && *(src - 1) != '/' ) {
        src--;
    }
    
    // copy up to eight characters
    memset( dest, 0, 8 );
    int length = 0;

    while ( *src && *src != '.' ) {
        if ( ++length == 9 ) {
            //TODO I_Error( "Filename base of %s >8 chars", path );
        }
        *dest++ = toupper( (int)*src++ );
    }
}

}
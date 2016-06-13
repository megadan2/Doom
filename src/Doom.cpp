#include "Doom.h"
#include "StringUtil.h"
#include "FileUtil.h"
#include "original/m_argv.h"
#include "original/d_main.h"
#include "original/i_system.h"
#include <io.h>

// File locations, relative to current position. Path names are OS-sensitive.
#define DEVMAPS "devmaps"
#define DEVDATA "devdata"

namespace Doom {

//=============================================================================
Doom::Doom( int argc, char** argv ) :
    m_cmdArgs( argc, argv ),
    m_gameMode( GameMode::Indetermined ),
    m_language( Language::English ),
    m_devParm( false )
{
    //TODO remove
    myargc = argc;
    myargv = argv;
}

//=============================================================================
void Doom::Run() {
    IdentifyVersion();
    D_DoomMain( this ); //TODO remove
}

//=============================================================================
// Checks availability of IWAD files by name, to determine whether 
// registered/commercial features should be executed (notably loading PWAD's).
//=============================================================================
void Doom::IdentifyVersion() {
    char* home;
    #ifdef _WINDOWS
        home = getenv( "USERPROFILE" );
    #else
        home = getenv("HOME");
    #endif
    if ( !home ) {
        I_Error( "Please set $HOME to your home directory" );
    }
    m_baseDefault = StringUtil::Format( "%s/.doomrc", home );

    // Handle devmode parameters
    if ( m_cmdArgs.HasArg( "-shdev" ) ) {
        m_gameMode = GameMode::Shareware;
        m_devParm = true;
        AddWad( DEVDATA"doom1.wad" );
        AddWad( DEVMAPS"data_se/texture1.lmp" );
        AddWad( DEVMAPS"data_se/pnames.lmp" );
        m_baseDefault = DEVDATA"default.cfg";
        return;
    }
    if ( m_cmdArgs.HasArg( "-regdev" ) ) {
        m_gameMode = GameMode::Registered;
        m_devParm = true;
        AddWad( DEVDATA"doom.wad" );
        AddWad( DEVMAPS"data_se/texture1.lmp" );
        AddWad( DEVMAPS"data_se/texture2.lmp" );
        AddWad( DEVMAPS"data_se/pnames.lmp" );
        m_baseDefault = DEVDATA"default.cfg";
        return;
    }
    if ( m_cmdArgs.HasArg( "-comdev" ) ) {
        m_gameMode = GameMode::Commercial;
        m_devParm = true;
        AddWad( DEVDATA"doom2.wad" );
        AddWad( DEVMAPS"cdata/texture1.lmp" );
        AddWad( DEVMAPS"cdata/pnames.lmp" );
        m_baseDefault = DEVDATA"default.cfg";
        return;
    }

    // Determine game mode by checking which wad is present
    char* doomWadDir = getenv( "DOOMWADDIR" );
    if ( !doomWadDir ) {
        doomWadDir = ".";
    }

    // French
    if ( CheckWad( doomWadDir, "doom2f.wad", GameMode::Commercial, Language::French ) ) {
        // C'est ridicule!
        // Let's handle languages in config files, okay?
        printf( "French version\n" );
        return;
    }
    if ( CheckWad( doomWadDir, "doom2.wad", GameMode::Commercial ) )    { return; } // Commercial
    if ( CheckWad( doomWadDir, "plutonia.wad", GameMode::Commercial ) ) { return; } // Plutonia (Final Doom)
    if ( CheckWad( doomWadDir, "tnt.wad", GameMode::Commercial ) )      { return; } // TNT (Final Doom)
    if ( CheckWad( doomWadDir, "doomu.wad", GameMode::Retail ) )        { return; } // Retail  
    if ( CheckWad( doomWadDir, "doom.wad", GameMode::Registered ) )     { return; } // Registered
    if ( CheckWad( doomWadDir, "doom1.wad", GameMode::Shareware ) )     { return; } // Shareware

    printf( "Game mode indeterminate.\n" );
    m_gameMode = GameMode::Indetermined;
}

//=============================================================================
void Doom::AddWad( const string& wad ) {
    m_wadFiles.push_back( wad );
}

//=============================================================================
bool Doom::CheckWad( const char* wadDir, const char* wad, GameMode gameMode, Language language ) {
    string wadFile = StringUtil::Format( "%s/%s", wadDir, wad );
    if ( FileUtil::HasReadAccess( wadFile ) ) {
        m_gameMode = gameMode;
        AddWad( wadFile );
        return true;
    }
    return false;
}

}
#include "Doom.h"
#include "StringUtil.h"
#include "FileUtil.h"
#include "Math.h"
#include "original/m_argv.h"
#include "original/d_main.h"
#include "original/i_system.h"
#include <io.h>
#include <direct.h>

// File locations, relative to current position. Path names are OS-sensitive.
#define DEVMAPS "devmaps"
#define DEVDATA "devdata"

namespace Doom {

//=============================================================================
Doom::Doom( int argc, char** argv ) :
    m_cmdArgs( argc, argv ),
    m_zAlloc( 6 * 1024 * 1024 )
{
    CVar::SetCVarSystem( &m_cvarSystem );

    //TODO remove
    myargc = argc;
    myargv = argv;
}

//=============================================================================
void Doom::Run() {
    setbuf( stdout, nullptr );
    IdentifyVersion();
    PrintVersion();
    HandleArgs();

    // init subsystems
    printf( "V_Init: allocate screens.\n" );
    m_video.Init();

    printf( "M_LoadDefaults: Load system defaults.\n" );
    m_cvarSystem.Load( m_config.defaultFile.c_str() );    // load before initing other systems

    printf( "W_Init: Init WADfiles.\n" );
    m_wadSystem.InitMultipleFiles( m_wadFiles );

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
    m_config.defaultFile = StringUtil::Format( "%s/.doomrc", home );

    // Handle devmode parameters
    if ( m_cmdArgs.HasArg( "-shdev" ) ) {
        m_config.gameMode = GameMode::Shareware;
        m_config.devParm = true;
        AddWad( DEVDATA"doom1.wad" );
        AddWad( DEVMAPS"data_se/texture1.lmp" );
        AddWad( DEVMAPS"data_se/pnames.lmp" );
        m_config.defaultFile = DEVDATA"default.cfg";
        return;
    }
    if ( m_cmdArgs.HasArg( "-regdev" ) ) {
        m_config.gameMode = GameMode::Registered;
        m_config.devParm = true;
        AddWad( DEVDATA"doom.wad" );
        AddWad( DEVMAPS"data_se/texture1.lmp" );
        AddWad( DEVMAPS"data_se/texture2.lmp" );
        AddWad( DEVMAPS"data_se/pnames.lmp" );
        m_config.defaultFile = DEVDATA"default.cfg";
        return;
    }
    if ( m_cmdArgs.HasArg( "-comdev" ) ) {
        m_config.gameMode = GameMode::Commercial;
        m_config.devParm = true;
        AddWad( DEVDATA"doom2.wad" );
        AddWad( DEVMAPS"cdata/texture1.lmp" );
        AddWad( DEVMAPS"cdata/pnames.lmp" );
        m_config.defaultFile = DEVDATA"default.cfg";
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
    m_config.gameMode = GameMode::Indetermined;
}

//=============================================================================
void Doom::PrintVersion() {
    string title;
    switch ( m_config.gameMode ) {
    case GameMode::Retail:
        title = StringUtil::Format(
                 "                         "
                 "The Ultimate DOOM Startup v%i.%i"
                 "                           ",
                 VERSION / 100, VERSION % 100 );
        break;
    case GameMode::Shareware:
        title = StringUtil::Format(
                 "                            "
                 "DOOM Shareware Startup v%i.%i"
                 "                           ",
                 VERSION / 100, VERSION % 100 );
        break;
    case GameMode::Registered:
        title = StringUtil::Format(
                 "                            "
                 "DOOM Registered Startup v%i.%i"
                 "                           ",
                 VERSION / 100, VERSION % 100 );
        break;
    case GameMode::Commercial:
        title = StringUtil::Format(
                 "                         "
                 "DOOM 2: Hell on Earth v%i.%i"
                 "                           ",
                 VERSION / 100, VERSION % 100 );
        break;
        /*FIXME
        case pack_plut:
        sprintf (title,
        "                   "
        "DOOM 2: Plutonia Experiment v%i.%i"
        "                           ",
        VERSION/100,VERSION%100);
        break;
        case pack_tnt:
        sprintf (title,
        "                     "
        "DOOM 2: TNT - Evilution v%i.%i"
        "                           ",
        VERSION/100,VERSION%100);
        break;
        */
    default:
        title = StringUtil::Format(
                 "                     "
                 "Public DOOM - v%i.%i"
                 "                           ",
                 VERSION / 100, VERSION % 100 );
        break;
    }

    printf( "%s\n", title.c_str() );
}

//=============================================================================
void Doom::HandleArgs() {
    m_config.noMonsters     = m_cmdArgs.HasArg( "-nomonsters" );
    m_config.respawnParm    = m_cmdArgs.HasArg( "-respawn" );
    m_config.fastParm       = m_cmdArgs.HasArg( "-fast" );
    m_config.devParm        = m_cmdArgs.HasArg( "-devparm" );
    m_config.deathmatch     = m_cmdArgs.HasArg( "-altdeath" ) ? 2 : m_cmdArgs.HasArg( "-deathmatch" ) ? 1 : 0;

    if ( m_config.devParm ) {
        printf( D_DEVSTR );
    }

    if ( m_cmdArgs.HasArg( "-cdrom" ) ) {
        printf( D_CDROM );
        _mkdir( "c:\\doomdata" );
        m_config.defaultFile = "c:/doomdata/default.cfg";
    }

    int config = m_cmdArgs.GetArgIndex( "-config" );
    if ( config ) {
        m_config.defaultFile = m_cmdArgs.GetArg( config + 1 );
    }

    if ( m_cmdArgs.HasArg( "-turbo" ) ) {
        m_config.turboScale = Mathi::Clamp( m_cmdArgs.GetArgAsInt( "-turbo", 200 ), 10, 400 );
        printf( "turbo scale: %i%%\n", m_config.turboScale );
    }

    // add any files specified on the command line with -file wadfile
    // to the wad list
    //
    // convenience hack to allow -wart e m to add a wad file
    // prepend a tilde to the filename so wadfile will be reloadable
    bool warp = false;
    int warpEpisode = 0;
    int warpMission = 0;
    int wart = m_cmdArgs.GetArgIndex( "-wart" );
    if ( wart ) {
        warp = true;
        string file;
        switch ( m_config.gameMode ) {
            case GameMode::Shareware:
            case GameMode::Retail:
            case GameMode::Registered: {
                warpEpisode = m_cmdArgs.GetArgAsInt( wart + 1 );
                warpMission = m_cmdArgs.GetArgAsInt( wart + 2 );
                file = StringUtil::Format( "~%sE%iM%i.wad", DEVMAPS, warpEpisode, warpMission );
                printf( "Warping to Episode %i, Map %i.\n", warpEpisode, warpMission );
                break;
            }

            case GameMode::Commercial:
            default: {
                warpMission = m_cmdArgs.GetArgAsInt( wart + 1 );
                if ( warpMission < 10 ) {
                    file = StringUtil::Format( "~%scdata/map0%i.wad", DEVMAPS, warpMission );
                } else {
                    file = StringUtil::Format( "~%scdata/map%i.wad", DEVMAPS, warpMission );
                }
                break;
            }
        }
        AddWad( file );
    }

    if ( m_cmdArgs.HasArg( "-file" ) ) {
        // the parms after -file are wadfile/lump names, until end of parms or another - preceded parm
        m_config.modifiedGame = true;   // homebrew levels
        for ( string& file : m_cmdArgs.GetArgParams( "-file" ) ) {
            AddWad( file );
        }
    }

    int index = m_cmdArgs.GetArgIndex( "-playdemo" );
    if ( !index ) {
        index = m_cmdArgs.GetArgIndex( "-timedemo" );
    }
    if ( index && index < m_cmdArgs.Size() - 1 ) {
        string file = StringUtil::Format( "%s.lmp", m_cmdArgs.GetArg( index + 1 ) );
        AddWad( file );
        printf( "Playing demo %s.lmp.\n", file.c_str() );
    }

    // get skill / episode / map from parms		
    index = m_cmdArgs.GetArgIndex( "-skill" );
    if ( index && index < m_cmdArgs.Size() - 1 ) {
        m_config.startSkill = (Skill)( m_cmdArgs.GetArgAsInt( index + 1 ) - 1 );
        m_config.autoStart = true;
    }

    index = m_cmdArgs.GetArgIndex( "-episode" );
    if ( index && index < m_cmdArgs.Size() - 1 ) {
        m_config.startEpisode = m_cmdArgs.GetArgAsInt( index + 1 );
        m_config.startMap = 1;
        m_config.autoStart = true;
    }

    index = m_cmdArgs.GetArgIndex( "-timer" );
    if ( index && index < m_cmdArgs.Size() - 1 && m_config.deathmatch ) {
        int time = m_cmdArgs.GetArgAsInt( index + 1 );
        printf( "Levels will end after %d minute", time );
        if ( time > 1 ) {
            printf( "s" );
        }
        printf( ".\n" );
    }

    index = m_cmdArgs.GetArgIndex( "-avg" );
    if ( index && index < m_cmdArgs.Size() - 1 && m_config.deathmatch ) {
        printf( "Austin Virtual Gaming: Levels will end after 20 minutes\n" );
    }

    index = m_cmdArgs.GetArgIndex( "-warp" );
    if ( index && index < m_cmdArgs.Size() - 1 ) {
        if ( m_config.gameMode == GameMode::Commercial ) {
            m_config.startMap = m_cmdArgs.GetArgAsInt( index + 1 );
        } else {
            m_config.startEpisode = m_cmdArgs.GetArgAsInt( index + 1 );
            m_config.startMap = m_cmdArgs.GetArgAsInt( index + 2 );
        }
        m_config.autoStart = true;
    }
}

//=============================================================================
void Doom::AddWad( const string& wad ) {
    m_wadFiles.push_back( wad );
}

//=============================================================================
bool Doom::CheckWad( const char* wadDir, const char* wad, GameMode gameMode, Language language ) {
    string wadFile = StringUtil::Format( "%s/%s", wadDir, wad );
    if ( FileUtil::HasReadAccess( wadFile ) ) {
        m_config.gameMode = gameMode;
        m_config.language = language;
        AddWad( wadFile );
        return true;
    }
    return false;
}

}
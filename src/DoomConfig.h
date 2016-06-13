#pragma once

#include "DoomDef.h"
#include <string>

using std::string;

namespace Doom {

struct DoomConfig {
                    DoomConfig();

    GameMode        gameMode;
    Language        language;
    string          baseDefault;
    bool            devParm;        // DEBUG: launched with -devparm
    bool            modifiedGame;   // Set if homebrew PWAD stuff has been added.
    bool            noMonsters;     // -nomonsters option
    bool            respawnParm;    // -respawn option 
    bool            fastParm;       // -fast option
    int             deathmatch;     // Flag: true only if started as net deathmatch. An enum might handle altdeath/cooperative better.
};

//=============================================================================
inline DoomConfig::DoomConfig() :
    gameMode( GameMode::Indetermined ),
    language( Language::English ),
    devParm( false ),
    modifiedGame( false ),
    noMonsters( false ),
    respawnParm( false ),
    fastParm( false )
{}

}

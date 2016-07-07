#pragma once

#include "DoomDef.h"
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace Doom {

struct DoomConfig {
                    DoomConfig();

    GameMode        gameMode;
    Language        language;
    Skill           startSkill      = Skill::Medium;
    int             startEpisode    = 1;
    int             startMap        = 1;
    bool            autoStart       = false;
    string          defaultFile;
    bool            devParm;                    // DEBUG: launched with -devparm
    bool            modifiedGame;               // Set if homebrew PWAD stuff has been added.
    bool            noMonsters;                 // -nomonsters option
    bool            respawnParm;                // -respawn option 
    bool            fastParm;                   // -fast option
    int             deathmatch;                 // Flag: true only if started as net deathmatch. An enum might handle altdeath/cooperative better.
    int             turboScale      = 100;      // -turbo
    vector<string>  wadFiles;
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

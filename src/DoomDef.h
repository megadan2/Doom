#pragma once

namespace Doom {

#define VERSION 110

// All important printed strings.
// Language selection (message strings).
// Use -DFRENCH etc.

#ifdef FRENCH
#include "languages/French.h"
#else
#include "languages/English.h"
#endif


//=============================================================================
// Game mode handling - identify IWAD version to handle IWAD dependend 
// animations etc.
//=============================================================================
enum class GameMode {
    Shareware,	    // DOOM 1 shareware, E1, M9
    Registered,	    // DOOM 1 registered, E3, M27
    Commercial,	    // DOOM 2 retail, E1 M34
                    // DOOM 2 german edition not handled
    Retail,         // DOOM 1 retail, E4, M36
    Indetermined    // Well, no IWAD found.
};

//=============================================================================
// Identify language to use, software localization.
//=============================================================================
enum class Language {
    English,
    French,
    German,
    Unknown
};

//=============================================================================
enum class Skill {
    Baby,
    Easy,
    Medium,
    Hard,
    Nightmare
};

// File locations, relative to current position. Path names are OS-sensitive.
#define DEVMAPS "devmaps"
#define DEVDATA "devdata"

}

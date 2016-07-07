#pragma once

#include "CmdArgs.h"
#include "DoomConfig.h"
#include "Video.h"
#include "CVarSystem.h"
#include "ZoneAllocator.h"
#include "WadSystem.h"

namespace Doom {

class Doom {
public:
                    Doom( int argc, char** argv );

    void            Run();

    vector<string>& GetWadFiles();
    DoomConfig*     GetConfig();
    Video*          GetVideo();

private:
    void            IdentifyVersion();
    void            PrintVersion();
    void            HandleArgs();
    void            AddWad( const string& wad );
    bool            CheckWad( const char* wadDir, const char* wad, GameMode gameMode, Language language = Language::English );

private:
    CmdArgs         m_cmdArgs;
    vector<string>  m_wadFiles;
    DoomConfig      m_config;
    Video           m_video;
    CVarSystem      m_cvarSystem;
    ZoneAllocator   m_zAlloc;
    WadSystem       m_wadSystem;
};

//=============================================================================
inline vector<string>& Doom::GetWadFiles() {
    return m_wadFiles;
}

//=============================================================================
inline DoomConfig* Doom::GetConfig() {
    return &m_config;
}

//=============================================================================
inline Video* Doom::GetVideo() {
    return &m_video;
}

}

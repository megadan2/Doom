#pragma once

#include "CmdArgs.h"
#include "DoomConfig.h"

namespace Doom {

class Doom {
public:
                    Doom( int argc, char** argv );

    void            Run();

    vector<string>& GetWadFiles();
    DoomConfig*     GetConfig();

private:
    void            IdentifyVersion();
    void            PrintVersion();
    void            AddWad( const string& wad );
    bool            CheckWad( const char* wadDir, const char* wad, GameMode gameMode, Language language = Language::English );

private:
    CmdArgs         m_cmdArgs;
    vector<string>  m_wadFiles;
    DoomConfig      m_config;
};

//=============================================================================
inline vector<string>& Doom::GetWadFiles() {
    return m_wadFiles;
}

//=============================================================================
inline DoomConfig* Doom::GetConfig() {
    return &m_config;
}

}

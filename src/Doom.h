#pragma once

#include "CmdArgs.h"
#include "DoomDef.h"

namespace Doom {

class Doom {
public:
                    Doom( int argc, char** argv );

    void            Run();

    vector<string>& GetWadFiles();
    GameMode        GetGameMode() const;
    Language        GetLanguage() const;
    string          GetBaseDefault() const;
    bool            IsDevMode() const;

private:
    void            IdentifyVersion();
    void            AddWad( const string& wad );
    bool            CheckWad( const char* wadDir, const char* wad, GameMode gameMode, Language language = Language::English );

private:
    CmdArgs         m_cmdArgs;
    vector<string>  m_wadFiles;
    GameMode        m_gameMode;
    Language        m_language;
    string          m_baseDefault;
    bool            m_devParm;  //DEBUG: launched with -devparm
};

//=============================================================================
inline vector<string>& Doom::GetWadFiles() {
    return m_wadFiles;
}

//=============================================================================
inline GameMode Doom::GetGameMode() const {
    return m_gameMode;
}

//=============================================================================
inline Language Doom::GetLanguage() const {
    return m_language;
}

//=============================================================================
inline string Doom::GetBaseDefault() const {
    return m_baseDefault;
}

//=============================================================================
inline bool Doom::IsDevMode() const {
    return m_devParm;
}

}

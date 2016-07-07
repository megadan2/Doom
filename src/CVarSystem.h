#pragma once

#include <unordered_map>
#include <string>
#include <memory>

using std::unordered_map;
using std::string;
using std::shared_ptr;

namespace Doom {

class CVarSystem;
class CVar;

using CVarPtr = shared_ptr<CVar>;

//=============================================================================
class CVar {
public:
                                    // Don't call this constructor
                                    CVar( const char* name, const char* value );

    const char*                     GetName() const;
    const char*                     GetString() const;
    int                             GetInt() const;

    void                            Set( const char* value );

public:
    static CVarPtr                  Get( const char* name, const char* value, bool setIfExists = false );
    static void                     SetCVarSystem( CVarSystem* cvarSystem );

private:
    const char*                     m_name;
    string                          m_value;
    int                             m_intValue;
    int                             m_scanTranslate;  // PC scan code hack
    int                             m_untranslated;   // lousy hack

private:
    static CVarSystem*              s_cvarSystem;
};

//=============================================================================
class CVarSystem {
public:
                                    CVarSystem();
    void                            Load( const char* file );
    void                            Register( const CVarPtr& cvar );
    CVarPtr                         Get( const char* name );

public:
    int                             mouseSensitivity;

private:
    unordered_map<string, CVarPtr>  m_cvars;
};

//=============================================================================
inline const char* CVar::GetName() const {
    return m_name;
}

//=============================================================================
inline const char* CVar::GetString() const {
    return m_value.c_str();
}

//=============================================================================
inline int CVar::GetInt() const {
    return m_intValue;
}

//=============================================================================
inline void CVar::Set( const char* value ) {
    m_value = value;
    m_intValue = atoi( value );
}

//=============================================================================
inline void CVar::SetCVarSystem( CVarSystem* cvarSystem ) {
    s_cvarSystem = cvarSystem;
}

//=============================================================================
inline CVarPtr CVarSystem::Get( const char* name ) {
    auto cvar = m_cvars.find( name );
    if ( cvar != m_cvars.end() ) {
        return cvar->second;
    }
    return nullptr;
}

}

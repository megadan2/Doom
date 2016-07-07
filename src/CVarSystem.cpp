#include "CVarSystem.h"
#include <stdio.h>

using std::make_shared;

namespace Doom {

CVarSystem* CVar::s_cvarSystem = nullptr;

//=============================================================================
CVar::CVar( const char* name, const char* value ) :
    m_name( name ),
    m_value( value ),
    m_intValue( atoi( value ) )
{

}

//=============================================================================
CVarPtr CVar::Get( const char* name, const char* value, bool setIfExists ) {
    if ( s_cvarSystem ) {
        CVarPtr cvar = s_cvarSystem->Get( name );
        if ( cvar ) { 
            if ( setIfExists ) {
                cvar->Set( value );
            }
            return cvar; 
        }
    }

    CVarPtr cvar = make_shared<CVar>( name, value );
    if ( s_cvarSystem ) {
        s_cvarSystem->Register( cvar );
    }
    return cvar;
}

//=============================================================================
CVarSystem::CVarSystem() {

}

//=============================================================================
void CVarSystem::Load( const char* file ) {
    printf( "	default file: %s\n", file );

    char name[80];
    char value[100];
    string strValue;

    // read the file in, overriding any set defaults
    FILE* f = fopen( file, "r" );
    if ( f ) {
        while ( !feof( f ) ) {
            if ( fscanf( f, "%79s %[^\n]\n", name, value ) == 2 ) {
                strValue = value;
                if ( value[0] == '"' ) {
                    strValue = strValue.substr( 1, strValue.length() - 2 );
                } else if ( value[0] == '0' && value[1] == 'x' ) {
                    strValue = strValue.substr( 2 );
                }

                CVar::Get( name, strValue.c_str(), true );
            }
        }

        fclose( f );
    }
}

//=============================================================================
void CVarSystem::Register( const CVarPtr& cvar ) {
    m_cvars[cvar->GetName()] = cvar;
}

}
#include "CmdArgs.h"
#include "TextFile.h"

namespace Doom {

//=============================================================================
CmdArgs::CmdArgs( int argc, char** argv ) {
    for ( int i = 0; i < argc; i++ ) {
        if ( argv[i][0] == '@' ) {
            printf( "Found response file %s!\n", &argv[i][1] );

            // Doom source replaces all parameters (except executable) before the @responseFile
            // with the contents of the file. All arguments after the response file are preserved
            if ( i > 1 ) {
                m_args.erase( m_args.begin() + 1 );
            }
            ReadResponseFile( &argv[i][1] );
        } else {
            m_args.emplace_back( argv[i] );
        }
    }

    printf( "%d command-line args:\n", m_args.size() );
    for ( string& arg : m_args ) {
        printf( "%s\n", arg.c_str() );
    }
}

//=============================================================================
bool CmdArgs::HasArg( const char* name ) const {
    return GetArgIndex( name ) > 0;
}

//=============================================================================
int CmdArgs::GetArgIndex( const char* name ) const {
    for ( size_t i = 1; i < m_args.size(); i++ ) {
        if ( !_stricmp( name, m_args[i].c_str() ) ) {
            return i;
        }
    }
    return 0;
}

//=============================================================================
int CmdArgs::GetArgBeginsWithIndex( const char* str ) const {
    for ( size_t i = 1; i < m_args.size(); i++ ) {
        if ( strstr( m_args[i].c_str(), str ) == m_args[i].c_str() ) {
            return i;
        }
    }
    return 0;
}

//=============================================================================
const char* CmdArgs::GetArg( const char* name ) const {
    int index = GetArgIndex( name );
    if ( index > 0 ) {
        return m_args[index].c_str();
    }
    return nullptr;
}

//=============================================================================
int CmdArgs::GetArgAsInt( const char* name, int default ) const {
    int index = GetArgIndex( name );
    if ( index > 0 ) {
        return atoi( m_args[index].c_str() );
    }
    return default;
}

//=============================================================================
void CmdArgs::ReadResponseFile( const char* file ) {
    TextFile textFile( file );
    vector<string> words = textFile.ReadAllWords();

    //TODO if file doesnt exist
    /*printf( "\nNo such response file!" );
    exit( 1 );*/

    for ( string& word : words ) {
        m_args.push_back( word );
    }
}

}
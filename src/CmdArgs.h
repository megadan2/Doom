#pragma once

#include <vector>
#include <string>
using std::vector;
using std::string;

namespace Doom {

class CmdArgs {
public:
                        CmdArgs( int argc, char** argv );

    bool                HasArg( const char* name ) const;
    int                 GetArgIndex( const char* name ) const;
    int                 GetArgBeginsWithIndex( const char* str ) const;
    const char*         GetArg( const char* name ) const;
    int                 GetArgAsInt( const char* name, int default=0 ) const;

private:
    void                ReadResponseFile( const char* file );

private:
    vector<string>      m_args;
};

}

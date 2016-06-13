#include "TextFile.h"
#include <fstream>
#include <sstream>

using std::ifstream;
using std::ios;
using std::istringstream;

namespace Doom {

//=============================================================================
TextFile::TextFile( const char* file ) :
    m_file( file )
{}

//=============================================================================
string TextFile::ReadAll() {
    ifstream ifs( m_file.c_str() );
    ifs.seekg( 0, ios::end );
    size_t size = (size_t)ifs.tellg();
    ifs.seekg( 0, ios::beg );
    vector<char> bytes( size );
    ifs.read( &bytes[0], size );
    return string( &bytes[0], size );
}

//=============================================================================
vector<string> TextFile::ReadAllWords() {
    string contents = ReadAll();
    istringstream stream( contents );
    string arg;
    vector<string> words;
    while ( stream >> arg ) {
        words.push_back( arg );
    }
    return words;
}

}
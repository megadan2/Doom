#pragma once

#include <string>
#include <vector>
using std::string;
using std::vector;

namespace Doom {

class TextFile {
public:
                    TextFile( const char* file );
    string          ReadAll();
    vector<string>  ReadAllWords();

private:
    string          m_file;
};

}

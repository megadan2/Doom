#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace Doom {

class WadSystem {
public:
            WadSystem();

    void    InitMultipleFiles( const vector<string>& files );

private:
    int     m_numLumps;
};

}

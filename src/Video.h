#pragma once

#include <stdint.h>
#include <memory>
#include "Rect.h"

using std::unique_ptr;

namespace Doom {

class Video {
public:
                            Video();

    void                    Init();
    void                    MarkRect( int x, int y, int width, int height );
    uint8_t*                GetScreen( int index );

public:
    static const int        Width   = 320;
    static const int        Height  = 200;

private:
    // Screen 0 is the screen updated by I_Update screen.
    // Screen 1 is an extra buffer.
    uint8_t                 m_screens[5][Width*Height];
    Rect                    m_dirtyBox;
};

//=============================================================================
inline uint8_t* Video::GetScreen( int index ) {
    return m_screens[index];
}

}

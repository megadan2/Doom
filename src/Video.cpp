#include "Video.h"

namespace Doom {

//=============================================================================
Video::Video() {

}

//=============================================================================
void Video::Init() {

}

//=============================================================================
void Video::MarkRect( int x, int y, int width, int height ) {
    m_dirtyBox.Add( (float)x, (float)y );
    m_dirtyBox.Add( (float)x + width - 1, (float)y + height - 1 );
}

}
#pragma once

#include <Windows.h>
#include <stdint.h>

namespace Doom {

class Window;

class Screen {
public:
                    Screen( Window* window, int width, int height );
                    ~Screen();

    void            Draw( uint8_t* buf );
    void            SetPalette( uint8_t* palette, int gammaTable );

private:
    Window*         m_window;
    int             m_width;
    int             m_height;
    HDC             m_windowDC;
    HDC             m_srcDC;
    HBITMAP         m_screen;
    uint32_t*       m_screenBuf;
    uint32_t        m_palette[256];
};

}
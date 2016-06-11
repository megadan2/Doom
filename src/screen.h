#pragma once

#include <Windows.h>

namespace Doom {

class Window;

class Screen {
public:
                    Screen( Window* window, int width, int height );
                    ~Screen();
    void            Draw(char* buf);

private:
    Window*         m_window;
    int             m_width;
    int             m_height;
    HBITMAP         m_screen;
    unsigned char*  m_screenBuf;
};

}
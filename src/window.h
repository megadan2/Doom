#pragma once

#include <Windows.h>

namespace Doom {

class Window {
public:
            Window( int width, int height );
    void    ProcessMessages();

private:
    LRESULT MessageHandler( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

    static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
    HWND    m_handle;
};

}

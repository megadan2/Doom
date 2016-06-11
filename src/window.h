#pragma once

#include <Windows.h>

namespace Doom {

class Window {
public:
            Window( int width, int height );
    void    ProcessMessages();

    HWND    GetHandle() const;
    int     GetWidth() const;
    int     GetHeight() const;

private:
    LRESULT MessageHandler( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

    static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
    HWND    m_handle;
    int     m_width;
    int     m_height;
};

//=============================================================================
inline HWND Window::GetHandle() const {
    return m_handle;
}

//=============================================================================
inline int Window::GetWidth() const {
    return m_width;
}

//=============================================================================
inline int Window::GetHeight() const {
    return m_height;
}

}

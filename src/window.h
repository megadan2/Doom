#pragma once

#include <Windows.h>
#include <functional>
#include "original/d_event.h"

namespace Doom {

using EventHandler = std::function<void(event_t*)>;

class Window {
public:
                    Window( int width, int height );
    void            ProcessMessages();

    HWND            GetHandle() const;
    int             GetWidth() const;
    int             GetHeight() const;

    void            SetEventHandler( EventHandler eventHandler );

private:
    LRESULT         MessageHandler( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );
    int             TranslateKey( WPARAM key );

    static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
    HWND            m_handle;
    int             m_width;
    int             m_height;
    EventHandler    m_eventHandler;
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

//=============================================================================
inline void Window::SetEventHandler( EventHandler eventHandler ) {
    m_eventHandler = eventHandler;
}

}

#include "window.h"

namespace Doom {

//=============================================================================
Window::Window( int width, int height ) {
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof( WNDCLASSEX );
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetModuleHandle( NULL );
    wcex.hIcon = NULL;
    wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
    wcex.hbrBackground = (HBRUSH)( COLOR_WINDOW + 1 );
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"DoomClass";
    wcex.hIconSm = NULL;

    if ( !RegisterClassEx( &wcex ) ) {
        //DRV: TODO
    }

    RECT rc = { 0, 0, width, height };
    AdjustWindowRect( &rc, WS_OVERLAPPEDWINDOW, FALSE );

    m_handle = CreateWindow(
        L"DoomClass",
        L"Doom",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        rc.right - rc.left,
        rc.bottom - rc.top,
        NULL,
        NULL,
        GetModuleHandle( NULL ),
        this );

    if ( !m_handle ) {
        //DRV: TODO
    }

    ShowWindow( m_handle, SW_SHOW );
}

//=============================================================================
void Window::ProcessMessages() {
    MSG msg;
    while ( PeekMessage( &msg, m_handle, 0, 0, PM_REMOVE ) ) {
        if ( msg.message == WM_QUIT ) {
        } else {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
    }
}

//=============================================================================
LRESULT Window::MessageHandler( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {
    switch ( message ) {
    case WM_DESTROY:
        PostQuitMessage( 0 );
        return 0;
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}

//=============================================================================
LRESULT CALLBACK Window::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam ) {
    Window* window = NULL;

    if ( message == WM_NCCREATE ) {
        // If the window is being created, set the user data to the window
        // pointer retrieved from the create params.
        LPCREATESTRUCT createStruct = (LPCREATESTRUCT)lParam;
        window = reinterpret_cast<Window*>( createStruct->lpCreateParams );
        SetWindowLongPtr( hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( window ) );
    } else {
        // Otherwise, retrieve the window pointer from the user data
        LONG_PTR windowAddress = GetWindowLongPtr( hWnd, GWLP_USERDATA );
        window = reinterpret_cast<Window*>( windowAddress );

        // Call the window's message handler to process the message
        if ( window ) {
            return window->MessageHandler( hWnd, message, wParam, lParam );
        }
    }

    return DefWindowProc( hWnd, message, wParam, lParam );
}

}
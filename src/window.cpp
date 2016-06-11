#include "window.h"
#include "doomdef.h"

namespace Doom {

//=============================================================================
Window::Window( int width, int height ) :
    m_width( width ),
    m_height( height )
{
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

        case WM_KEYDOWN: {
            event_t event;
            event.type = ev_keydown;
            event.data1 = TranslateKey( wParam );
            if (m_eventHandler) {
                m_eventHandler( &event );
            }
            return 0;
        }

        case WM_KEYUP: {
            event_t event;
            event.type = ev_keyup;
            event.data1 = TranslateKey( wParam );
            if ( m_eventHandler ) {
                m_eventHandler( &event );
            }
            return 0;
        }
    }
    return DefWindowProc( hWnd, message, wParam, lParam );
}

//=============================================================================
int Window::TranslateKey( WPARAM wParam ) {
    int rc = wParam;
    switch ( wParam ) {
        case VK_LEFT:       rc = KEY_LEFTARROW;	    break;
        case VK_RIGHT:      rc = KEY_RIGHTARROW;    break;
        case VK_DOWN:       rc = KEY_DOWNARROW;     break;
        case VK_UP:         rc = KEY_UPARROW;       break;
        case VK_ESCAPE:     rc = KEY_ESCAPE;        break;
        case VK_RETURN:     rc = KEY_ENTER;         break;
        case VK_TAB:	    rc = KEY_TAB;           break;
        case VK_F1:         rc = KEY_F1;            break;
        case VK_F2:         rc = KEY_F2;            break;
        case VK_F3:         rc = KEY_F3;            break;
        case VK_F4:         rc = KEY_F4;            break;
        case VK_F5:         rc = KEY_F5;            break;
        case VK_F6:         rc = KEY_F6;            break;
        case VK_F7:         rc = KEY_F7;            break;
        case VK_F8:         rc = KEY_F8;            break;
        case VK_F9:         rc = KEY_F9;            break;
        case VK_F10:        rc = KEY_F10;           break;
        case VK_F11:        rc = KEY_F11;           break;
        case VK_F12:        rc = KEY_F12;           break;
        case VK_BACK:
        case VK_DELETE:     rc = KEY_BACKSPACE;     break;
        case VK_PAUSE:      rc = KEY_PAUSE;         break;
        //case XK_KP_Equal:
        //case XK_equal:	rc = KEY_EQUALS;	break;
        //
        //case XK_KP_Subtract:
        //case XK_minus:	rc = KEY_MINUS;		break;
        case VK_SHIFT:
        case VK_LSHIFT:
        case VK_RSHIFT:     rc = KEY_RSHIFT;        break;
        case VK_CONTROL:
        case VK_LCONTROL:
        case VK_RCONTROL:   rc = KEY_RCTRL;         break;
        //     case XK_Alt_L:
        //     case XK_Meta_L:
        //     case XK_Alt_R:
        //     case XK_Meta_R:
        //rc = KEY_RALT;
        //break;
        //
        default:
            //if (rc >= XK_space && rc <= XK_asciitilde)
            //    rc = rc - XK_space + ' ';
            if (rc >= 'A' && rc <= 'Z')
                rc = rc - 'A' + 'a';
            break;
    }
    return rc;
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
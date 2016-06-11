#include "screen.h"
#include "window.h"

namespace Doom {

//=============================================================================
Screen::Screen( Window* window, int width, int height ) :
    m_window( window ),
    m_width( width ),
    m_height( height )
{
    HDC hdcWindow = GetDC( window->GetHandle() );
    
    BITMAPINFO bmi;
    bmi.bmiHeader.biSize = sizeof( BITMAPINFO );
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height; // Order pixels from top to bottom
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32; // last byte not used, 32 bit for alignment
    bmi.bmiHeader.biCompression = BI_RGB;
    bmi.bmiHeader.biSizeImage = 0;
    bmi.bmiHeader.biXPelsPerMeter = 0;
    bmi.bmiHeader.biYPelsPerMeter = 0;
    bmi.bmiHeader.biClrUsed = 0;
    bmi.bmiHeader.biClrImportant = 0;
    bmi.bmiColors[0].rgbBlue = 0;
    bmi.bmiColors[0].rgbGreen = 0;
    bmi.bmiColors[0].rgbRed = 0;
    bmi.bmiColors[0].rgbReserved = 0;

    m_screen = CreateDIBSection( hdcWindow, &bmi, DIB_RGB_COLORS, (void**)&m_screenBuf, NULL, 0 );

    //for (int i = 0; i < 200*4; i+=4) {
    //    m_screenBuf[200 * width + i + 0] = 0x00; // B
    //    m_screenBuf[200 * width + i + 1] = 0x00; // G
    //    m_screenBuf[200 * width + i + 2] = 0xff; // R
    //    m_screenBuf[200 * width + i + 3] = 0x00; // X
    //}


    ReleaseDC( NULL, hdcWindow );
}

//=============================================================================
Screen::~Screen() {
    //free( m_screenBuf );
}

//=============================================================================
void Screen::Draw( char* buf ) {
   
    for (int i = 0, j = 0; i < m_width * m_height; i++, j+=4) {
        m_screenBuf[j] = 0x00; // B
        m_screenBuf[j+1] = 0x00; // G
        m_screenBuf[j+2] = buf[i]; // R
        m_screenBuf[j+3] = 0x00; // X
    }

    HDC hdcWindow = GetDC( m_window->GetHandle() );
    HDC hdcSrc = CreateCompatibleDC( hdcWindow );
    HBITMAP hbmOld = static_cast<HBITMAP>( SelectObject( hdcSrc, m_screen ) );

    StretchBlt( hdcWindow, 0, 0, m_window->GetWidth(), m_window->GetHeight(), hdcSrc, 0, 0, m_width, m_height, SRCCOPY );

    SelectObject( hdcSrc, hbmOld );
    DeleteDC( hdcSrc );
    ReleaseDC( NULL, hdcWindow );
}

}
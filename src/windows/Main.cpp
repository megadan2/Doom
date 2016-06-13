#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "../Doom.h"

//=============================================================================
void InitializeConsole() {
    AllocConsole();
    freopen( "CON", "w", stdout );
    freopen( "CON", "w", stderr );
    freopen( "CON", "r", stdin );
}

//=============================================================================
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd ) {
    InitializeConsole();
    Doom::Doom doom( __argc, __argv );
    doom.Run();
    return 0;
}
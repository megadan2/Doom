#pragma once

#include <cfloat>

class Rect {
public:
    void    Clear();
    void    Add( float x, float y );

public:
    float   top;
    float   left;
    float   right;
    float   bottom;
};

//=============================================================================
inline void Rect::Clear() {
    top     = -FLT_MAX;
    left    =  FLT_MAX;
    right   = -FLT_MAX;
    bottom  =  FLT_MAX;
}

//=============================================================================
inline void Rect::Add( float x, float y ) {
    if ( x < left ) {
        left = x;
    } else if ( x > right ) {
        right = x;
    }
    if ( y < bottom ) {
        bottom = y;
    } else if ( y > top ) {
        top = y;
    }
}
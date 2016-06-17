#pragma once

namespace Doom {

template <class T>
class Math {
public:
    static T Clamp( const T& v, const T& lo, const T& hi );
};

using Mathf = Math<float>;
using Mathi = Math<int>;

//=============================================================================
template <class T>
inline T Math<T>::Clamp( const T& v, const T& lo, const T& hi ) {
    return v < lo ? lo : v > hi ? hi : v;
}

}

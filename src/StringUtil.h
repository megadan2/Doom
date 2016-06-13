#pragma once

#include <string>
#include <memory>
using std::string;
using std::unique_ptr;

namespace Doom {

class StringUtil {
public:
    template<class... Args>
    static string Format( const string& format, Args... args);

};

//=============================================================================
template<class... Args>
inline string StringUtil::Format( const string& format, Args... args ) {
    size_t size = snprintf( nullptr, 0, format.c_str(), args... ) + 1;
    unique_ptr<char[]> buf = std::make_unique<char[]>( size );
    snprintf( buf.get(), size, format.c_str(), args... );
    return string( buf.get(), buf.get() + size - 1 );
}

}
//	Endianess handling, swapping 16bit and 32bit.

#pragma once

// Endianess handling.
// WAD files are stored little endian.
#ifdef __BIG_ENDIAN__

namespace Doom {
    short	SwapSHORT( short );
    long	SwapLONG( long );
}
#define SHORT(x) ((short)Doom::SwapSHORT((unsigned short) (x)))
#define LONG(x)  ((long)Doom::SwapLONG((unsigned long) (x)))
#else
#define SHORT(x) (x)
#define LONG(x)  (x)
#endif

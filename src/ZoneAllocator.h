#pragma once

#include <stdint.h>

namespace Doom {

// Purge Tags
// Tags < 100 are not overwritten until freed.
enum class ZonePurge {
    Static      = 1,    // static entire execution time
    Sound       = 2,    // static while playing
    Music       = 3,    // static while playing
    Dave        = 4,    // anything else Dave wants static
    Level       = 50,   // static until level exited
    LevSpec     = 51,   // a special thinker in a level
    // Tags >= 100 are purgable whenever needed.
    PurgeLevel  = 100,
    Cache       = 101
};

struct MemBlock {
    int             size;   // including the header and possibly tiny fragments
    void**          user;   // NULL if a free block
    ZonePurge       tag;    // purgelevel
    int             id;	    // should be ZONEID
    MemBlock*       next;
    MemBlock*       prev;
};

struct MemZone {
    int             size;   // total bytes malloced, including header
    MemBlock        blocklist; // start / end cap for linked list
    MemBlock*       rover;
};

class ZoneAllocator {
public:
                    ZoneAllocator( uint32_t sizeMB );

private:
    MemZone*        m_mainZone;
    uint32_t        m_sizeMB;

};

}

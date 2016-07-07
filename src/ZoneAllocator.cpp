#include "ZoneAllocator.h"
#include <stdio.h>

namespace Doom {

//=============================================================================
ZoneAllocator::ZoneAllocator( uint32_t sizeMB ) :
    m_sizeMB( sizeMB )
{
    printf( "Z_Init: Init zone memory allocation daemon. \n" );

    uint8_t* mem = new uint8_t[m_sizeMB];
    m_mainZone = (MemZone*)mem;
    m_mainZone->size = m_sizeMB;

    // set the entire zone to one free block
    MemBlock* freeBlock = (MemBlock*)( mem + sizeof( MemZone ) );
    m_mainZone->blocklist.next = freeBlock;
    m_mainZone->blocklist.prev = freeBlock;

    m_mainZone->blocklist.user = (void**)m_mainZone;
    m_mainZone->blocklist.tag = ZonePurge::Static;
    m_mainZone->rover = freeBlock;

    freeBlock->prev = freeBlock->next = &m_mainZone->blocklist;

    // NULL indicates a free block.
    freeBlock->user = nullptr;

    freeBlock->size = m_mainZone->size - sizeof( MemZone );
}

}
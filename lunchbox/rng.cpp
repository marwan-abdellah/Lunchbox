
/* Copyright (c) 2010-2012, Stefan Eilemann <eile@equalizergraphics.com> 
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 2.1 as published
 * by the Free Software Foundation.
 *  
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "rng.h"

#pragma warning (push)
#pragma warning (disable: 4985) // inconsistent decl of ceil

#ifdef _WIN32
#  ifndef NOMINMAX
#    define NOMINMAX
#  endif
#  include <wtypes.h>
#  include <wincrypt.h>
#  pragma comment(lib, "advapi32.lib")
#else
#  include <unistd.h>
#endif

#include <cstdlib>
#include <fcntl.h>
#include <limits>
#include <stdio.h>
#pragma warning (pop)


namespace lunchbox
{
namespace
{
#ifdef __linux__
static int _fd = -1;
#elif defined (_WIN32)
static HCRYPTPROV _provider = 0;
#endif
}

RNG::RNG()
        : _impl( 0 )
{
    _init();
}

RNG::~RNG()
{}

bool RNG::_init()
{
#ifdef __APPLE__
    srandomdev();
#elif defined (__linux__)
    static int fd = -1; // prevent static initializer fiasco
    if( fd >= 0 )
        return true;

    fd = ::open( "/dev/urandom", O_RDONLY );
    if( fd >= 0 )
        ::atexit( RNG::_exit );
    else
    {
        LBERROR << "Failed to open /dev/urandom: " << sysError << std::endl;
        return false;
    }
    _fd = fd;

#elif defined (_WIN32)

    static HCRYPTPROV provider = 0; // prevent static initializer fiasco
    if( provider )
        return true;

    if( CryptAcquireContext( &provider, 0, 0, PROV_RSA_FULL,
                              CRYPT_VERIFYCONTEXT ) || !provider )
    {
        ::atexit( RNG::_exit );
    }
    else
    {
        LBERROR << "Failed to acquire crypto context: " << sysError <<std::endl;
        return false;
    }

    _provider = provider;
#endif
    return true;
}

void RNG::_exit()
{
#ifdef __linux__
    if( _fd >= 0 )
    {
        ::close( _fd );
        _fd = -1;
    }
#elif defined (_WIN32)
    if( _provider && !CryptReleaseContext( _provider, 0 ))
        LBERROR << "Failed to release crypto context: " << sysError
                << std::endl;
    _provider = 0;
#endif
}

void RNG::reseed()
{
#ifdef __APPLE__
    srandomdev();
#endif
}

bool RNG::_get( void* data, const size_t size )
{
#ifdef __linux__
    LBASSERTINFO( _fd >= 0, "init() not called?" );
    int read = ::read( _fd, data, size );
    LBASSERTINFO( read == ssize_t( size ),
                  read << " != " << size << ": " << sysError );
    if( read != ssize_t( size ))
    {
        LBERROR << "random number generator not working" << std::endl;
        return false;
    }

#elif defined (_WIN32)
    LBASSERTINFO( _provider, "init() not called?" );
    if( !CryptGenRandom( _provider, size, (BYTE*)data ))
    {
        LBERROR << "random number generator not working" << std::endl;
        return false;
    }
#else // __APPLE__
    uint8_t* ptr = reinterpret_cast< uint8_t* >( data );
    for( size_t i=0; i < size; ++i )
        ptr[i] = ( random() & 0xff );
#endif
    return true;
}

}

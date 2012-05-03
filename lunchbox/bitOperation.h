
/* Copyright (c) 2010, Cedric Stalder <cedric.stalder@gmail.com>
 *               2011-2012, Stefan Eilemann <eile@eyescale.ch>
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

#ifndef LUNCHBOX_BITOPERATION_H
#define LUNCHBOX_BITOPERATION_H

#include <lunchbox/types.h>
#ifdef _MSC_VER
#  pragma warning (push)
#  pragma warning (disable: 4985) // inconsistent decl of ceil
#    include <intrin.h>
#  pragma warning (pop)
#endif

namespace lunchbox
{
    /** @return the position of the last (most significant) set bit, or -1. */
    template< class T > int32_t getIndexOfLastBit( T value );

    template<> inline int32_t getIndexOfLastBit< uint32_t >( uint32_t value )
    {
#ifdef __APPLE__
        return ::fls( value ) - 1;
#elif defined __GNUC__
        return value ? (31 - __builtin_clz( value )) : -1;
#elif defined _MSC_VER
        unsigned long i = 0;
        return _BitScanReverse( &i, value ) ? i : -1;
#else
        int32_t count = -1;
        while( value ) 
        {
          ++count;
          value >>= 1;
        }
        return count;
#endif
    }

    template<> inline int32_t getIndexOfLastBit< uint64_t >( uint64_t value )
    {
#ifdef  __GNUC__
        return value ? (63 - __builtin_clzll( value )) : -1;
#elif defined _WIN64
        unsigned long i = 0;
        return _BitScanReverse64( &i, value ) ? i : -1;
#else
        int32_t count = -1;
        while( value ) 
        {
          ++count;
          value >>= 1;
        }
        return count;
#endif
    }

#if defined(__linux__) && defined(_LP64)
    template<> inline int32_t 
    getIndexOfLastBit< unsigned long long >( unsigned long long value )
        { return getIndexOfLastBit( static_cast< uint64_t >( value )); }
#endif
#ifdef __APPLE__
#  ifdef _LP64
    template<> inline
    int32_t getIndexOfLastBit< unsigned long >( unsigned long value )
        { return getIndexOfLastBit( static_cast< uint64_t >( value )); }
#  else
    template<> inline
    int32_t getIndexOfLastBit< unsigned long >( unsigned long value )
        { return getIndexOfLastBit( static_cast< uint32_t >( value )); }
#  endif
#endif
}
#endif //LUNCHBOX_BITOPERATION_H


/* Copyright (c) 2007-2012, Stefan Eilemann <eile@equalizergraphics.com> 
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

#ifndef LUNCHBOX_DEBUG_H
#define LUNCHBOX_DEBUG_H

#include <lunchbox/defines.h>
#include <lunchbox/log.h>

#include <typeinfo>

// assertions
// #define LB_RELEASE_ASSERT

namespace lunchbox
{
/**
 * @internal
 * Used to trap into an infinite loop to allow debugging of assertions
 */
LUNCHBOX_API void abort();

/**
 * @internal
 * Check the consistency of the heap and abort on error (Win32 only).
 */
LUNCHBOX_API void checkHeap();

/** 
 * Print a textual description of the current system error.
 *
 * The current system error is OS-specific, e.g., errno or GetLastError().
 * @version 1.0
 */
LUNCHBOX_API std::ostream& sysError( std::ostream& os );

/** 
 * Print the current call stack.
 *
 * May not be implemented on all platforms.
 * @version 1.0
 */
LUNCHBOX_API std::ostream& backtrace( std::ostream& os );

LUNCHBOX_API std::string demangleTypeID( const char* mangled ); //!< @internal

/** Print the RTTI name of the given class. @version 1.0 */
template< class T > inline std::string className( T* object )
    { return demangleTypeID( typeid( *object ).name( )); }

}

#ifdef NDEBUG
#  ifdef LB_RELEASE_ASSERT
#    define LBASSERT(x)                                                 \
    {                                                                   \
        if( !(x) )                                                      \
            LBERROR << "##### Assert: " << #x << " #####" << std::endl  \
                    << lunchbox::forceFlush;                            \
        lunchbox::checkHeap();                                          \
    }
#    define LBASSERTINFO(x, info)                                       \
    {                                                                   \
        if( !(x) )                                                      \
            LBERROR << "##### Assert: " << #x << " [" << info << "] #####" \
                    << std::endl << lunchbox::forceFlush;               \
        lunchbox::checkHeap();                                          \
    }
#    define LBCHECK(x) { const bool eqOk = x; LBASSERTINFO( eqOk, #x ) }
#  else
#    define LBASSERT(x)
#    define LBASSERTINFO(x, info)
#    define LBCHECK(x) { x; }
#  endif

#  define LBUNIMPLEMENTED { LBERROR << "Unimplemented code" << std::endl \
                                    << lunchbox::forceFlush; }
#  define LBUNREACHABLE   { LBERROR << "Unreachable code" << std::endl  \
                                    << lunchbox::forceFlush; }
#  define LBDONTCALL                                                    \
    { LBERROR << "Code is not supposed to be called in this context"    \
              << std::endl << lunchbox::forceFlush; }
#  define LBABORT( info ) {                                         \
        LBERROR << "##### Abort: " << info << " #####" << std::endl \
                << lunchbox::forceFlush; }

#else // NDEBUG

#  define LBASSERT(x)                                                   \
    {                                                                   \
        if( !(x) )                                                      \
        {                                                               \
            LBERROR << "Assert: " << #x << " ";                         \
            lunchbox::abort();                                          \
        }                                                               \
        lunchbox::checkHeap();                                          \
    } 
#  define LBASSERTINFO(x, info)                                         \
    {                                                                   \
        if( !(x) )                                                      \
        {                                                               \
            LBERROR << "Assert: " << #x << " [" << info << "] ";        \
            lunchbox::abort();                                          \
        }                                                               \
        lunchbox::checkHeap();                                          \
    }

#  define LBUNIMPLEMENTED                                               \
    { LBERROR << "Unimplemented code in " << lunchbox::className( this ) \
              << " ";                                                   \
        lunchbox::abort(); }
#  define LBUNREACHABLE                                                 \
    { LBERROR << "Unreachable code in " << lunchbox::className( this )  \
              << " ";                                                   \
        lunchbox::abort(); }
#  define LBDONTCALL                                                    \
    { LBERROR << "Code is not supposed to be called in this context, type " \
              << lunchbox::className( this ) << " " ;                   \
        lunchbox::abort(); }

#  define LBCHECK(x) { const bool eqOk = x; LBASSERTINFO( eqOk, #x ) }
#  define LBABORT( info ) {                                             \
        LBERROR << "Abort: " << info;                                   \
        lunchbox::abort(); }
#endif // NDEBUG

#define LBSAFECAST( to, in ) static_cast< to >( in );   \
    LBASSERT( in == 0 || dynamic_cast< to >( static_cast< to >( in )))

#endif //LUNCHBOX_DEBUG_H

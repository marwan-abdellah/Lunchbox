 
/* Copyright (c) 2005-2012, Stefan Eilemann <eile@equalizergraphics.com> 
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

/** Defines export visibility macros for lunchbox. */
#ifndef LUNCHBOX_API_H
#define LUNCHBOX_API_H

#include <lunchbox/defines.h>

#if defined(_MSC_VER) || defined(__declspec)
#  define LB_DLLEXPORT __declspec(dllexport)
#  define LB_DLLIMPORT __declspec(dllimport)
#else // _MSC_VER
#  define LB_DLLEXPORT
#  define LB_DLLIMPORT
#endif // _MSC_VER

#if defined(LUNCHBOX_STATIC)
#  define LUNCHBOX_API
#elif defined(LUNCHBOX_SHARED)
#  define LUNCHBOX_API LB_DLLEXPORT
#else
#  define LUNCHBOX_API LB_DLLIMPORT
#endif

#endif //LUNCHBOX_API_H

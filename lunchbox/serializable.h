
/* Copyright (c) 2012, EFPL/Blue Brain Project
 *                     Daniel Nachbaur <daniel.nachbaur@epfl.ch>
 *
 * This file is part of Lunchbox <https://github.com/BlueBrain/Lunchbox>
 *
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License version 3.0 as published
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

#ifndef LUNCHBOX_SERIALIZABLE_H
#define LUNCHBOX_SERIALIZABLE_H

#include <lunchbox/defines.h>
#ifdef LUNCHBOX_USE_BOOST_SERIALIZATION

#  include <boost/serialization/access.hpp>
#  include <boost/serialization/split_member.hpp>

/** Makes a class to be serializable using boost.serialization. */
#  define LB_SERIALIZABLE                                       \
    friend class boost::serialization::access;                  \
    template< class Archive >                                   \
    void save( Archive& ar, const unsigned int version ) const; \
    template< class Archive >                                   \
    void load( Archive& ar, const unsigned int version );       \
    BOOST_SERIALIZATION_SPLIT_MEMBER()

#else
#  define LB_SERIALIZABLE
#endif

#endif // LUNCHBOX_SERIALIZABLE_H

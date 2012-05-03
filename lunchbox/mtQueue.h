
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

#ifndef LUNCHBOX_MTQUEUE_H
#define LUNCHBOX_MTQUEUE_H

#include <lunchbox/condition.h>
#include <lunchbox/debug.h>

#include <algorithm>
#include <limits.h>
#include <queue>
#include <string.h>

namespace lunchbox
{
    /**
     * A thread-safe queue with a blocking read access.
     *
     * Typically used to communicate between two execution threads.
     *
     * S defines the maximum capacity of the Queue<T>. When the capacity is
     * reached, pushing new values blocks until items have been consumed.
     */
    template< typename T, size_t S = ULONG_MAX > class MTQueue
    // S = std::numeric_limits< size_t >::max() does not work:
    //   http://gcc.gnu.org/bugzilla/show_bug.cgi?id=6424
    {
    public:
        /** Construct a new queue. @version 1.0 */
        MTQueue() {}

        /** Construct a copy of a queue. @version 1.0 */
        MTQueue( const MTQueue< T, S >& from ) : _queue( from._queue ) {}

        /** Destruct this Queue. @version 1.0 */
        ~MTQueue() {}

        /** Assign the values of another queue. @version 1.0 */
        MTQueue< T, S >& operator = ( const MTQueue< T, S >& from )
            {
                _cond.lock();
                _queue = from._queue;
                _cond.signal();
                _cond.unlock();
                return *this;
            }

        /** @return true if the queue is empty, false otherwise. @version 1.0 */
        bool isEmpty() const { return _queue.empty(); }

        /** @return the number of items currently in the queue. @version 1.0 */
        size_t getSize() const { return _queue.size(); }

        /**
         * Wait for the size to be at least the number of given elements.
         *
         * @return the current size when the condition was fulfilled.
         * @version 1.0
         */
        size_t waitSize( const size_t minSize ) const
            {
                LBASSERT( minSize <= S );
                _cond.lock();
                while( _queue.size() < minSize )
                    _cond.wait();
                const size_t size = _queue.size();
                _cond.unlock();
                return size;
            }

        /** Reset (empty) the queue. @version 1.0 */
        void clear()
            {
                _cond.lock();
                _queue.clear();
                _cond.unlock();
            }

        /** 
         * Retrieve and pop the front element from the queue, may block.
         * @version 1.0
         */
        T pop()
            {
                _cond.lock();
                while( _queue.empty( ))
                    _cond.wait();
                
                LBASSERT( !_queue.empty( ));
                T element = _queue.front();
                _queue.pop_front();
                _cond.signal();
                _cond.unlock();
                return element;
            }

        /** 
         * Retrieve and pop the front element from the queue.
         *
         * @param timeout the timeout
         * @param element the element returned
         * @return true if an element was popped
         * @throws Exception on timeout.
         * @version 1.1
         */
        bool timedPop( const unsigned timeout, T& element )
            {
                _cond.lock();
                while( _queue.empty( ))
                {
                    if( !_cond.timedWait( timeout ))
                    {
                        _cond.unlock();
                        return false;
                    }
                }
                LBASSERT( !_queue.empty( ));
                element = _queue.front();
                _queue.pop_front();
                _cond.signal();
                _cond.unlock();
                return true;
            }

        /** 
         * Retrieve and pop the front element from the queue if it is not empty.
         *
         * @param result the front value or unmodified.
         * @return true if an element was placed in result, false if the queue
         *         is empty.
         * @version 1.0
         */
        bool tryPop( T& result )
            {
                _cond.lock();
                if( _queue.empty( ))
                {
                    _cond.unlock();
                    return false;
                }

                result = _queue.front();
                _queue.pop_front();
                _cond.signal();
                _cond.unlock();
                return true;
            }   

        /** 
         * Try to retrieve a number of items for the front of the queue.
         *
         * Between zero and the given number of items are appended to the
         * vector.
         *
         * @param num the maximum number of items to retrieve
         * @param result the front value or unmodified.
         * @return true if an element was placed in result, false if the queue
         *         is empty.
         * @version 1.1.6
         */
        void tryPop( const size_t num, std::vector< T >& result )
            {
                _cond.lock();
                const size_t size = LB_MIN( num, _queue.size( ));
                if( size > 0 )
                {
                    result.reserve( result.size() + size );
                    for( size_t i = 0; i < size; ++i )
                    {
                        result.push_back( _queue.front( ));
                        _queue.pop_front();
                    }
                    _cond.signal();
                }
                _cond.unlock();
            }   

        /** 
         * @param result the front value or unmodified.
         * @return true if an element was placed in result, false if the queue
         *         is empty.
         * @version 1.0
         */
        bool getFront( T& result ) const
            {
                _cond.lock();
                if( _queue.empty( ))
                {
                    _cond.unlock();
                    return false;
                }
                // else
                result = _queue.front();
                _cond.unlock();
                return true;
            }

        /** 
         * @param result the last value or unmodified.
         * @return true if an element was placed in result, false if the queue
         *         is empty.
         * @version 1.0
         */
        bool getBack( T& result ) const
            {
                _cond.lock();
                if( _queue.empty( ))
                {
                    _cond.unlock();
                    return false;
                }
                // else
                result = _queue.back();
                _cond.unlock();
                return true;
            }

        /** Push a new element to the back of the queue. @version 1.0 */
        void push( const T& element )
            {
                _cond.lock();
                while( _queue.size() >= S )
                    _cond.wait();
                _queue.push_back( element );
                _cond.signal();
                _cond.unlock();
            }

        /** Push a vector of elements to the back of the queue. @version 1.0 */
        void push( const std::vector< T >& elements )
            {
                _cond.lock();
                LBASSERT( elements.size() <= S );
                while( (S - _queue.size( )) < elements.size( ))
                    _cond.wait();
                _queue.insert( _queue.end(), elements.begin(), elements.end( ));
                _cond.signal();
                _cond.unlock();
            }

        /** Push a new element to the front of the queue. @version 1.0 */
        void pushFront( const T& element )
            {
                _cond.lock();
                while(_queue.size() >= S)
                    _cond.wait();
                _queue.push_front( element );
                _cond.signal();
                _cond.unlock();
            }

        /** Push a vector of elements to the front of the queue. @version 1.0 */
        void pushFront( const std::vector< T >& elements )
            {
                _cond.lock();
                LBASSERT( elements.size() <= S );
                while( (S - _queue.size( )) < elements.size( ))
                    _cond.wait();
                _queue.insert(_queue.begin(), elements.begin(), elements.end());
                _cond.signal();
                _cond.unlock();
            }


    private:
        std::deque< T > _queue;
        mutable Condition _cond;
    };
}
#endif //LUNCHBOX_MTQUEUE_H

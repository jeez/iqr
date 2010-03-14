/****************************************************************************
 **
 ** $Author: Mark Blanchard
 ** 
 ** $Description: Ring buffer template.
 **
 **               This template constructs a ring buffer using objects
 **               of the requested class.  An index to the current
 **               element is managed, and the user can move this
 **               pointer to the next element in the buffer explicitly
 **               using advance().  When indexing the elements of the
 **               buffer, [0] refers to the current element, [1] to
 **               the previous element etc.
 **
 *****************************************************************************/

#ifndef RINGBUFFER_HPP
#define RINGBUFFER_HPP

#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

namespace iqrcommon {

    class InvalidRingBufferIndex : public runtime_error
    {
    public:
	/** 
	 * Constructor for invalid ring buffer index exception class.
	 */
	InvalidRingBufferIndex()
	    : runtime_error(string("invalid index for ring buffer"))
	{}
    };

    template <class T> class ClsRingBuffer
    {
    public:
	/** 
	 * Constructs a ring buffer with at least RB_MINIMUM_SIZE
	 * elements.
	 *
	 * The elements in the buffer are constructed automatically
	 * using the default constructor of class T.
	 *
	 * @param _size Number of elements in buffer.
	 */
	ClsRingBuffer(unsigned int _size = RB_DEFAULT_SIZE)
	    : buffer(_size > RB_MINIMUM_SIZE ? _size : RB_MINIMUM_SIZE),
	      current(0)
	{ }

	~ClsRingBuffer() 
	{ }

	/** 
	 * Get the buffer size.
	 * 
	 * @return Number of elements in the buffer.
	 */
	unsigned int getSize() const 
	{
	    return buffer.size();
	}

	/** 
	 * Set the buffer size.
	 *
	 * When the buffer size is increased, new elements are added
	 * to the buffer using the default constructor of type T.
	 * Elements are deleted when the size of the buffer decreases.
	 *
	 * @param _size New size for the buffer.  Values of 1 or
	 * greater are valid, values less than 1 are ignored (the
	 * buffer is not resized).
	 */
	void setSize(unsigned int _size)
	{
	    if (_size < 1) {
		// Ignore invalid sizes
		return;
	    }
	    buffer.resize(_size);
	    // Ensure the current index refers to a valid element.
	    if (current >= getSize()) {
		current = getSize() - 1;
	    }
	}

	/** 
	 * Subscript operator provides access to buffer elements.
	 *
	 * The current element is treated as [0].  Previous elements
	 * are accessed [1], [2], etc. in sequence.
	 *
	 * If an invalid index is received (>= size of buffer), an
	 * exception is thrown.
	 *
	 * @return Reference to the requested element.
	 * @param _index Index of element.
	 * @exception InvalidRingBufferIndex
	 *   - index to invalid element (>= size of buffer)
	 */
	T& operator[] (unsigned int _index)
	{
	    if (_index >= getSize()) {
		throw InvalidRingBufferIndex();
	    }
	    
	    int index = current - _index;
	    if (index < 0) {
		index += getSize();
	    }

	    return buffer[index];
	}

	/**
	 * Overloaded const verson of subscript operator.
	 *
	 * const volatile indicates that the values referenced to are
	 * not constant, only read-only.
	 */
// 	const volatile T& operator[] (unsigned int _index) const
// 	const T& operator[] (unsigned int _index) const
// 	{
// 	    if (_index >= getSize()) {
// 		throw InvalidRingBufferIndex();
// 	    }
	    
// 	    int index = current - _index;
// 	    if (index < 0) {
// 		index += getSize();
// 	    }
// 	    return buffer[index];
// 	}

	/** 
	 * Increment the current element index.
	 */
	void advance()
	{
	    ++current;

	    // Loop at the end of the buffer.
	    current = current % getSize();
	}

    private:
	static const unsigned int RB_DEFAULT_SIZE = 1;
	static const unsigned int RB_MINIMUM_SIZE = 1;

	vector<T> buffer;
	unsigned int current;
    };
}

#endif

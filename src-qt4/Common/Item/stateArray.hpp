/****************************************************************************
 **
 ** $Author: Mark Blanchard
 ** 
 ** $Description: A state array is a ring buffer of valarrays.  The
 **               type of data stored in the valarrays is specified
 **               via the template parameter.
 **
 **               The dimensions of a state array are the width,
 **               corresponding to the number of elements in each
 **               valarray, and the depth, corresponding to the number
 **               of valarrays stored in the underlying ring buffer.
 **               The depth allows a history of previous states to be
 **               maintained.
 **
 **               At present IQR only supports computation using
 **               doubles.  The typedef here shields the more awkward
 **               template syntax underlying the state array
 **               implementation.  The underlying template machinery
 **               may prove useful in later versions of IQR, where
 **               more data types are supported.  In this version, no
 **               additional runtime cost is incurred as templates are
 **               initialized at compile time (the only cost is slower
 **               compilation).
 **
 **               std::valarrays are used as a compromise between
 **               computational efficiency and cross-platform
 **               compatibility.  Other more specialized libraries
 **               (for example, Blitz++) could be used here later.
 **
 *****************************************************************************/

#ifndef STATEARRAY_HPP
#define STATEARRAY_HPP

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <valarray>
#include <iterator> //MARK
using namespace std;

#include "ringBuffer.hpp"

namespace iqrcommon {

    template <class T> class ClsStateArray;
    /**
     * Use this typedef when creating state arrays in IQR.  At
     * present, only computation in doubles is supported.
     */
    typedef ClsStateArray<double> StateArray;

    template <class T> class ClsStateArray
    {
    public:
	/**
	 * Constructor for a state array.
	 *
	 * The type stored is specified by the template parameter T.
	 *
	 * @param _width Width of the valarrays, which corresponds to
	 * the number of elements which can be updated
	 * simulataneously.  Values less than DEFAULT_WIDTH are
	 * ignored.
	 * @param _depth Depth of the ring buffer.
	 */
	ClsStateArray(unsigned int _width = DEFAULT_WIDTH, 
		      unsigned int _depth = DEFAULT_DEPTH)
	    : buffer(_depth)
	{
	    unsigned int width = _width;

	    if (width < MINIMUM_WIDTH) {
		width = MINIMUM_WIDTH;
	    }

	    // Resize the valarrays stored in the ring buffer.
	    for (unsigned int i = 0; i < buffer.getSize(); ++i) {
		buffer[i].resize(width);
	    }
	}

	/** 
	 * Extend the number of valarrays in the underlying ring
	 * buffer.
	 *
	 * Any new valarrays created are initialized to the correct
	 * number of elements.
	 *
	 * @param _iNewLength New length of state array ring buffer.
	 * Sizes less than the present size as well as sizes < 1 are
	 * ignored.
	 */
	// 	void extend(int _iNewLength)
	// 	{
	// 	    int iOldLength = buffer.getLength();
	
	// 	    // If the new length is not longer, do nothing (the ring
	// 	    // buffer should never be made shorter.
	// 	    if (_iNewLength > iOldLength) {
	// 		buffer.changeLength(_iNewLength);
	// 		for (int i = iOldLength; i < _iNewLength; ++i) {
	// 		    // Get the size of the valarrays from the current
	// 		    // element in the ring buffer.
	// 		    buffer[i].resize(buffer[0].size());
	// 		}
	// 	    }
	// 	}
	
	/** 
	 * Set width of the valarray.
	 * 
	 * The depth of the ring buffer remains constant.
	 *
	 * @param _width Width of valarrays in number of elements.  If
	 * the specified width is less than the minimum width, this
	 * function does nothing.
	 */	
	void setWidth(unsigned int _width)
	{
	    setSize(_width, getDepth());
	}

	/** 
	 * Set the depth of the ring buffer.
	 * 
	 * The width of the valarrays remains constant.  Any new
	 * valarrays are initialized to the correct width.
	 *
	 * @param _depth Depth of the ring buffer, i.e. number of
	 * valarrays.  If the specified depth is less than the minimum
	 * depth, this function does nothing.
	 */
	void setDepth(unsigned int _depth)
	{
	    setSize(getWidth(), _depth);
	}
	
	/** 
	 * Set the size of the state array.
	 *
	 * @param _width Width of valarrays in number of elements.  If
	 * the specified width is less than the minimum width, the
	 * width is not changed.
	 * @param _depth Depth of the ring buffer, i.e. number of
	 * valarrays.  If the specified depth is less than the minimum
	 * depth, the depth is not changed.
	 */
	void setSize(unsigned int _width, unsigned int _depth)
	{
	    unsigned int oldWidth = getWidth(), oldDepth = getDepth();
	    unsigned int width    = _width,     depth    = _depth;

	    if (width < MINIMUM_WIDTH) {
		width = oldWidth;
	    }

	    if (depth < MINIMUM_DEPTH) {
		depth = oldDepth;
	    }

	    if (depth != oldDepth) {
		buffer.setSize(depth);
	    }
	    // Set width of valarrays only when new valarrays have
	    // been created (depth has increased) or the width of the
	    // valarrays should change.
	    if (width != oldWidth || depth > oldDepth) {
		for (unsigned int i = 0; i < buffer.getSize(); ++i) {
		    buffer[i].resize(width);
		}
	    }
	}

	/** 
	 * Resets the size of the state array to the default.
	 */
	void resetSize()
	{
	    setSize(DEFAULT_WIDTH, DEFAULT_DEPTH);
	    /* at reset, we want to reset all buffers to 0 */
	    for (unsigned int i = 0; i < buffer.getSize(); ++i) {
		buffer[i] = 0;
	    }
	}

	/** 
	 * Subscript operators provide access to the requested element.
	 *
	 * Checking of the index is left to the ring buffer.  If the
	 * index is invalid an exception is thrown.  This function
	 * makes no attempt to catch the exception, so the caller must
	 * include a try/catch construct.
	 *
	 * @return Requested element.
	 * @param _index Index of the requested element.  [0] is the
	 * current element, [1] the previous and so on until size()-1.
	 */
 	valarray<T>& operator[] (unsigned int _index)
	{
	    return buffer[_index];
	}

	/**
	 * Overloaded const version of subscript operator.
	 *
	 * const volatile indicates that the values referenced to are
	 * not constant, only read-only.
	 */
// 	const volatile valarray<T>& operator[] (unsigned int _index) const
// 	const valarray<T>& operator[] (unsigned int _index) const
// 	{
// 	    return buffer[_index];
// 	}
	
	/**
	 * Conversion operator provides access to current element in
	 * buffer.
	 * 
	 * NOTE: this function might lead to unexpected errors.  It
	 * should be checked carefully.
	 */
// 	operator T& () 
// 	{
// 	    return (*this)[CURRENT];
// 	}

	/**
	 * Overloaded const version of conversion operator.
	 *
	 * const volatile indicates that the values referenced to are
	 * not constant, only read-only.
	 */
// 	const volatile operator T& () const 
// 	{
// 	    return this*[CURRENT];
// 	}

	/** 
	 * Advance the current index in the ring buffer.
	 *
	 * This function must be called explicitly at the start of
	 * each time step.
	 */
	void advance()
	{
	    buffer.advance();
	}

	/** 
	 * @return Depth of the state array, i.e. number of valarrays
	 * in ring buffer.
	 */
	unsigned int getDepth()
	{
	    return buffer.getSize();
	}

	/** 
	 * @return Width of the state array, i.e. number of elements
	 * in valarrays.
	 */
	unsigned int getWidth()
	{
	    return buffer[0].size();
	}

	/*
	 * Fill the current valarray ([0]) with random values between
	 * 0 and 1.
	 */
	void fillRandom()
	{
	    for (unsigned int i = 0; i < getWidth(); ++i) {
		buffer[0][i] = (double)rand() / (double)RAND_MAX;
	    }
	}

	/*
	 * Fill the current valarray ([0]) with 0 or 1 using the
	 * specified probability of 1 occuring.
	 */
	void fillProbabilityMask(double probability)
	{
	    fillRandom();

	    // Ensure that no errors occur for probability=1 by
	    // scaling the random values to 0<=value<1.  Use a value
	    // just under 1, we only need to ensure that probability 1
	    // gives a mask with all 1s.
	    static const double PROBABILITY_MASK_SCALE = 0.99999999;
	    buffer[0] *= PROBABILITY_MASK_SCALE;

	    // For probability=1, no values in the random buffer are
	    // less than 1-P=0 => no entries will be 0.0
	    buffer[0][buffer[0] < (1.0 - probability)] = 0.0;
	    // For probability=0, the scaling above ensures that no
	    // random value is equal to 1-P=1 => no entries will be 1.0

	    // For probability=1, all values in the buffer are at
	    // least equal to 1-P=0 => all entries are 1.0
	    buffer[0][buffer[0] >=  (1.0 - probability)] = 1.0;	    
	}

    private:
	static const unsigned int DEFAULT_WIDTH = 1;
	static const unsigned int DEFAULT_DEPTH = 1;
	static const unsigned int MINIMUM_WIDTH = 1;
	static const unsigned int MINIMUM_DEPTH = 1;
	static const unsigned int CURRENT = 0;

	ClsRingBuffer<valarray<T> > buffer;
    };
};

/** 
 * Operator to stream the contents of a valarray onto an output
 * stream.
 *
 * @return Reference to the output stream.
 * @param &_stream Output stream.
 * @param &_state Reference to the valarray.
 */
// template <class T> std::ostream& operator<<(std::ostream &_stream, 
//     					    const std::valarray<T> &_buffer)
template <class T> ostream& operator<<(ostream &_stream, 
				       const valarray<T> &_buffer)
{	
    for (unsigned int i = 0; i < _buffer.size(); ++i) {
    	_stream << _buffer[i] << ";";
    }
    
    return _stream;
};

/** 
 * Operator to stream the current element of a state array onto an
 * output stream.
 *
 * @return Reference to the output stream.
 * @param &_stream Output stream.
 * @param &_state Reference to the state array.
 */
// template <class T> std::ostream& operator<<(std::ostream &_stream, 
// 					    const iqrcommon::ClsStateArray<T> &_state)
template <class T> ostream& operator<<(ostream &_stream, 
				       const iqrcommon::ClsStateArray<T> &_state)
{	
    _stream << _state[0];

    return _stream;
};


//    /** 
//     * Operator to stream data into a valarray.
//     * 
//     * @return Reference to the input stream.
//     * @param &_stream Input stream containing the new data as a list of
//     * floating point numbers separated by semicolons.
//     * @param &_buffer Reference to the destination valarray.
//     */
// //    template <class T> std::istream& operator>>(std::istream &_stream, 
// //    					    const std::valarray<T> &_buffer)
//    template <class T> istream& operator>>(istream &_stream, 
//    					    const valarray<T> &_buffer)
//    {	
//        int i = 0;
//        // Clear buffer.  If the stream is GUARANTEED to have sufficient
//        // data for the buffer, this step can be omitted.
//        _buffer = 0.0;
  
//        while (i < _buffer.size() && _stream >> _buffer[i]) {
//    	// @@@ 12/12/01 How can the semicolons in the message be
//    	// ignored here?
//    	++i;
//        }

//        return _stream;
//    };

/** 
 * Operator to stream the current element of a state array onto an
 * output stringstream.  This uses the operators for valarrays.
 * 2002/08/15 ulysses: changed to stringstream
 *
 * @return Reference to the output stream.
 * @param &_stream Output stream.
 * @param &_state Reference to the state array.
 */
// template <class T> std::ostringstream& operator<<(std::ostringstream &_stream, 
//                                                   const iqrcommon::ClsStateArray<T> &_state)
template <class T> ostringstream& operator<<(ostringstream &_stream, 
					     const iqrcommon::ClsStateArray<T> &_state)
{	
    _stream << _state[0];
    return _stream;
};

/** 
 * Operator to stream data into a state array.
 * 
 * This operator puts the data in the string stream into the current element
 * in the state array. This uses the operators for valarrays.
 * 2002/08/15 ulysses: changed to stringstream
 *
 * @return Reference to the input stream.
 * @param &_stream Input stream containing the new data as a list of
 * floating point numbers separated by semicolons.
 * @param &_state Reference to the destination state array.
 */
// template <class T> std::istringstream& operator>>(std::istringstream &_stream, 
// 						  const iqrcommon::ClsStateArray<T> &_state)
template <class T> istringstream& operator>>(istringstream &_stream, 
					     const iqrcommon::ClsStateArray<T> &_state)
{	
    _stream >> _state[0];
    return _stream;
};


/* *************************************************************************************
   2002/08/06 ulysses
   string stream operators with fixed field sizes */


/** 
 * Operator to stream the contents of a valarray onto an output
 * string stream.
 *
 * @return Reference to the output string stream.
 * @param &_stream Output string stream.
 * @param &_state Reference to the valarray.
 */
// template <class T> std::ostringstream& operator<<(std::ostringstream &_stream, 
// 						  const std::valarray<T> &_buffer) 
template <class T> ostringstream& operator<<(ostringstream &_stream, 
					     const valarray<T> &_buffer) 
{	
  for (unsigned int i = 0; i < _buffer.size(); ++i) {
    //    _stream << _buffer[i] << ";";
    _stream << setw(sizeof(T)*8) << _buffer[i];
  }
  return _stream;
};


/** 
 * Operator to stream string data into a valarray. This uses the field length of the valarry type
 * 
 * @return Reference to the input string stream.
 * @param &_stream Input string stream containing the new data.
 * @param &_buffer Reference to the destination valarray.
 */
// template <class T> std::istringstream& operator>>(std::istringstream &_stream, 
// 						  std::valarray<T> &_buffer) 
template <class T> istringstream& operator>>(istringstream &_stream, 
					     valarray<T> &_buffer) 
{	
  unsigned int i = 0;
  _buffer = 0.0;
  
//   std::istream_iterator<T,char> inpos(_stream), endpos;
  istream_iterator<T,char> inpos(_stream), endpos;
  while(inpos!=endpos){
    _buffer[i] = *inpos;
    ++inpos;
    ++i;
  }
    return _stream;
};



#endif




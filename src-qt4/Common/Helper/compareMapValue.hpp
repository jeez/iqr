#ifndef COMPAREMAPVALUE_HPP
#define COMPAREMAPVALUE_HPP

#include <map>
// using std::pair;
using namespace std;

namespace iqrcommon {
    /**
     * Class template allowing the value of entries in a map to be
     * compared against a known value.
     *
     * The following code finds the first entry equal to value in
     * amap:
     *
     *     entry = find_if(amap.begin(), amap.end(), 
     *                     CompareMapValue<key_type,value_type>(value));
     *
     *
     * This functionality is useful when the value, as opposed to the
     * key, of a map entry is known and the key needs to be found.
     *
     * See Josuttis(1999), "The C++ Standard Library", pp. 211-212.
     */
    template <class Key, class Value>
    class CompareMapValue {	    
	public:
	CompareMapValue(const Value& _value) : value(_value) {}
	bool operator() (pair<Key,Value> entry) {
	    return entry.second == value;
	}
    private:
	Value value;
    };
}

#endif

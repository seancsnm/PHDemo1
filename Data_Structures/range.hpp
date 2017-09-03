#ifndef _BBB_RANGE_H_
#define _BBB_RANGE_H_

#include <climits>
#include <algorithm>
#include <string>
#include <iostream>

/**
 * Range calculator class for unsigned integers.
 */
class UIntRange
{
protected:
	unsigned int min;
	unsigned int max;
	
public:
	UIntRange();

	/**
	 * Updates the minimum and maximum values of this range to extend to at
	 * least the given val.
	 */
	void apply(unsigned int val);

	/**
	 * The minimum value seen by this instance.
	 * Starts as UINT_MAX.
	 */
	unsigned int getMin();

	/**
	 * The maximum value seen by this instance.
	 * Starts as 0.
	 */
	unsigned int getMax();

	friend std::ostream& operator<<(std::ostream& os, const UIntRange& range);
};

#endif
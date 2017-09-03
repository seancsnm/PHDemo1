#include "range.hpp"

UIntRange::UIntRange()
{
	min = UINT_MAX;
	max = (unsigned int) 0;
}

void UIntRange::apply(unsigned int val)
{
	min = std::min(min, val);
	max = std::max(max, val);
}

unsigned int UIntRange::getMin()
{
	return min;
}

unsigned int UIntRange::getMax()
{
	return max;
}

std::ostream& operator<<(std::ostream& os, const UIntRange& range)
{
	os << "[" << range.min << "," << range.max << "]";
	return os;
}
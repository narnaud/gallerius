#ifndef UTILITY_H
#define UTILITY_H

#include <algorithm>

namespace Utility {

template <typename T>
bool contains(const T &container, const typename T::value_type &value)
{
    typename T::const_iterator end = container.cend();
    typename T::const_iterator it = std::find(container.cbegin(), end, value);
    return it != end;
}
}

#endif // UTILITY_H

#include "bigint.h"
#include "memtrace.h"
#include <cstring>
#include <string>
template <unsigned int bits>
Bigint<bits>::Bigint(const unsigned long &x)
{
    if (bits % 64 != 0)
        throw("wrong size");
    storage = new unsigned long[bits / sizeof(unsigned long)];
    std::memset(storage, 0, sizeof(storage));
    storage[0] = x;
}
template <unsigned int bits>
Bigint<bits>::Bigint(const char *const &x)
{
    unsigned short number_of_runs = (strlen(x) + 15) / 16;
    for (int i = 0; i < number_of_runs; ++i)
    {
        sscanf(x + i * sizeof(char), "%16X", storage[i]);
    }
}

template class Bigint<>;
template class Bigint<2048>;
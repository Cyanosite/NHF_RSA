#ifndef BIGINT_H
#define BIGINT_H
#include <iostream>
template <unsigned int bits = 64>
class Bigint
{
    unsigned long *storage;

public:
    Bigint(const unsigned long & = 0);
    Bigint(const char *const &);
    void print()
    {
        std::cout << storage[0];
    }
    ~Bigint();
};

#endif
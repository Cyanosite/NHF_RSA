#ifndef BIGINT_H
#define BIGINT_H
#include <iostream>
template <unsigned int bits = 64>
struct Bigint
{
    unsigned long long *storage;
    Bigint(const unsigned long long & = 0);
    Bigint(const char *const &);
    Bigint(const Bigint &);
    ~Bigint();
    void print() const;
    Bigint &operator=(const Bigint &);
    unsigned long long &operator[](const unsigned int &) const;
    Bigint operator+(const Bigint &) const;
};

#endif
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
    Bigint operator-(const Bigint &) const;
};
template <unsigned int bits>
Bigint<bits>::Bigint(const unsigned long long &x)
{
    if (bits % 64 != 0)
        throw("wrong size");
    storage = new unsigned long long[bits / sizeof(unsigned long long)]{0};
    storage[0] = x;
}
template <unsigned int bits>
Bigint<bits>::Bigint(const char *const &x)
{
    storage = new unsigned long long[bits / sizeof(unsigned long long)]{0};
    unsigned short number_of_runs = (strlen(x) + 15) / 16;
    if (number_of_runs > 1)
    {
        const char *startpos = x + strlen(x) * sizeof(char) - 16 * sizeof(char);
        int i = 0;
        for (; i < number_of_runs - 1; ++i)
        {
            sscanf(startpos - i * 16, "%16llX", &storage[i]);
        }
        char read_this[7] = "%";
        sprintf(read_this + 1 * sizeof(char), "%dllX", (int)strlen(x) % 16);
        sscanf(x, read_this, &storage[i]);
    }
    else
        sscanf(x, "%llX", &storage[0]);
}
template <unsigned int bits>
Bigint<bits>::Bigint(const Bigint &x)
{
    storage = new unsigned long long[bits / sizeof(unsigned long long)]{0};
    std::memcpy(storage, x.storage, sizeof(x.storage) * sizeof(unsigned long long));
}
template <unsigned int bits>
Bigint<bits>::~Bigint()
{
    delete[] storage;
}
template <unsigned int bits>
void Bigint<bits>::print() const
{
    unsigned short size = bits / 64;
    std::cout << std::hex;
    for (int i = size - 1; i >= 0; --i)
    {
        if (storage[i] != 0)
            std::cout << storage[i];
    }
    std::cout << std::dec << std::endl;
}
template <unsigned int bits>
Bigint<bits> &Bigint<bits>::operator=(const Bigint &x)
{
    delete[] storage;
    storage = new unsigned long long[bits / sizeof(unsigned long long)]{0};
    std::memcpy(storage, x.storage, sizeof(x.storage) * sizeof(unsigned long long));
    return *this;
}
template <unsigned int bits>
unsigned long long &Bigint<bits>::operator[](const unsigned int &i) const
{
    return storage[i];
}
template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator+(const Bigint &x) const
{
    Bigint res(x);
    unsigned long long carry = 0;
    for (unsigned int i = 0; i < bits / 64; ++i)
    {
        res[i] += storage[i] + carry;
        carry = (storage[i] & x[i] & 0xC000000000000000) == 0xC000000000000000;
    }
    if (carry)
        throw("out of range");
    return res;
}
template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator-(const Bigint &) const
{
    
}

#endif
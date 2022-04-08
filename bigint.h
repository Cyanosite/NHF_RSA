#ifndef BIGINT_H
#define BIGINT_H
#include <iostream>
template <unsigned int bits = 64>
struct Bigint
{
    unsigned int *storage;
    Bigint(const unsigned int & = 0);
    Bigint(const char *const &);
    Bigint(const Bigint &);
    ~Bigint();
    Bigint &operator=(const Bigint &);
    bool operator==(const Bigint &) const;
    bool operator!=(const Bigint &) const;
    unsigned int &operator[](const unsigned int &) const;
    Bigint operator+(const Bigint &) const;
    Bigint operator-(const Bigint &) const;
    Bigint operator*(const Bigint &) const;
};
template <unsigned int bits>
Bigint<bits>::Bigint(const unsigned int &x)
{
    if (bits % sizeof(unsigned int) != 0)
        throw("wrong size");
    storage = new unsigned int[bits / sizeof(unsigned int)]{0};
    storage[0] = x;
}
template <unsigned int bits>
Bigint<bits>::Bigint(const char *const &x)
{
    if (bits % sizeof(unsigned int) != 0)
        throw("wrong size");
    storage = new unsigned int[bits / sizeof(unsigned int)]{0};
    unsigned short number_of_runs = (strlen(x) + 7) / 8;
    if (number_of_runs > 1)
    {
        const char *startpos = x + strlen(x) * sizeof(char) - 8 * sizeof(char);
        int i = 0;
        for (; i < number_of_runs - 1; ++i)
        {
            sscanf(startpos - i * 8, "%8X", &storage[i]);
        }
        char read_this[7] = "%";
        sprintf(read_this + 1 * sizeof(char), "%dX", (int)strlen(x) % 8);
        sscanf(x, read_this, &storage[i]);
    }
    else
        sscanf(x, "%X", &storage[0]);
}
template <unsigned int bits>
Bigint<bits>::Bigint(const Bigint &x)
{
    storage = new unsigned int[bits / sizeof(unsigned int)]{0};
    std::memcpy(storage, x.storage, sizeof(x.storage) * sizeof(unsigned int));
}
template <unsigned int bits>
Bigint<bits>::~Bigint()
{
    delete[] storage;
}
template <unsigned int bits>
std::ostream &operator<<(std::ostream &os, const Bigint<bits> &x)
{
    unsigned short size = bits / (sizeof(unsigned int) * 8);
    os << std::hex;
    bool isnull = true;
    for (int i = size - 1; i >= 0; --i)
    {
        if (isnull)
        {
            if (x[i] != 0)
            {
                os << x[i];
                isnull = false;
            }
        }
        else
            os << x[i];
    }
    if (isnull)
        os << 0;
    os << std::dec;
    return os;
}
template <unsigned int bits>
Bigint<bits> &Bigint<bits>::operator=(const Bigint &x)
{
    delete[] storage;
    storage = new unsigned int[bits / sizeof(unsigned int)]{0};
    std::memcpy(storage, x.storage, sizeof(x.storage) * sizeof(unsigned int));
    return *this;
}
template <unsigned int bits>
bool Bigint<bits>::operator==(const Bigint &x) const
{
    for (unsigned int i = 0; i < bits / (sizeof(unsigned int) * 8); ++i)
    {
        if (storage[i] != x.storage[i])
            return false;
    }
    return true;
}
template <unsigned int bits>
bool Bigint<bits>::operator!=(const Bigint &x) const
{
    for (unsigned int i = 0; i < bits / (sizeof(unsigned int) * 8); ++i)
    {
        if (storage[i] != x.storage[i])
            return true;
    }
    return false;
}
template <unsigned int bits>
unsigned int &Bigint<bits>::operator[](const unsigned int &i) const
{
    return storage[i];
}
template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator+(const Bigint &x) const
{
    Bigint res(x);
    unsigned short carry = 0;
    unsigned long long temp;
    for (unsigned int i = 0; i < bits / (sizeof(unsigned int) * 8); ++i)
    {
        temp = (unsigned long long)res[i] + (unsigned long long)storage[i] + (unsigned long long)carry;
        carry = temp >> (8 * sizeof(unsigned int));
        res[i] = (unsigned int)temp;
    }
    if (carry)
        throw("overflow");
    return res;
}
template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator-(const Bigint &x) const
{
    Bigint res(x);
    unsigned short borrow = 0;
    unsigned long long temp;
    for (unsigned int i = 0; i < bits / (sizeof(unsigned int) * 8); ++i)
    {
        temp = (unsigned long long)res[i] - ((unsigned long long)storage[i] + (unsigned long long)borrow);
        borrow = temp >> (8 * sizeof(unsigned int));
        res[i] = (unsigned int)temp;
    }
    if (borrow)
        throw("underflow");
    return res;
}
template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator*(const Bigint &x) const
{
    Bigint res;
    unsigned short carry = 0;
    unsigned short k = 0;
    unsigned long long temp;
    for (unsigned int i = 0; i < bits / (sizeof(unsigned int) * 8); ++i)
    {
        for (unsigned int j = 0; j < bits / (sizeof(unsigned int) * 8); ++j)
        {
            k = i + j;
            temp = (unsigned long long)storage[i] * (unsigned long long)x.storage[j] + (unsigned long long)carry;
            carry = temp >> (8 * sizeof(unsigned int));
            res[k] += (unsigned int)temp;
        }
        res[k] = carry;
    }
    if (carry)
        throw("overflow");
    return res;
}
#endif
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
    bool operator<(const Bigint &) const;
    bool operator<=(const Bigint &) const;
    bool operator>(const Bigint &) const;
    bool operator>=(const Bigint &) const;
    unsigned int &operator[](const unsigned int &) const;
    unsigned int num_bits(const Bigint &) const;
    bool is_even(const Bigint &) const;
    Bigint operator+(const Bigint &) const;
    Bigint operator-(const Bigint &) const;
    Bigint operator*(const Bigint &) const;
    Bigint operator/(const Bigint &) const;
    Bigint operator%(const Bigint &) const;
    Bigint operator<<(const unsigned int &) const;
    Bigint operator>>(const unsigned int &) const;
};
template <unsigned int bits>
Bigint<bits>::Bigint(const unsigned int &x)
{
    if (bits % (sizeof(unsigned int) * 8) != 0)
        throw("wrong size");
    storage = new unsigned int[bits / (sizeof(unsigned int) * 8)]{0};
    storage[0] = x;
}
template <unsigned int bits>
Bigint<bits>::Bigint(const char *const &x)
{
    if (bits % (sizeof(unsigned int) * 8) != 0)
        throw("wrong size");
    storage = new unsigned int[bits / (sizeof(unsigned int) * 8)]{0};
    unsigned short number_of_runs = (strlen(x) + 7) / 8;
    if (number_of_runs > 1)
    {
        const char *startpos = x + strlen(x) * sizeof(char) - 8 * sizeof(char);
        int i = 0;
        for (; i < number_of_runs - 1; ++i)
        {
            startpos -= 8;
            sscanf(startpos, "%8X", &storage[i]);
        }
        if (strlen(x) % 8 != 0)
        {
            char read_this[7] = "%";
            sprintf(read_this + 1 * sizeof(char), "%dX", (int)strlen(x) % 8);
            sscanf(x, read_this, &storage[i]);
        }
    }
    else
        sscanf(x, "%X", &storage[0]);
}
template <unsigned int bits>
Bigint<bits>::Bigint(const Bigint &x)
{
    storage = new unsigned int[bits / (sizeof(unsigned int) * 8)]{0};
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
            os << std::setw(8) << std::setfill('0') << (unsigned int)x[i];
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
    storage = new unsigned int[bits / (sizeof(unsigned int) * 8)]{0};
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
unsigned int Bigint<bits>::num_bits(const Bigint &) const
{
    int j = 0;
    for (int i = bits / (sizeof(unsigned int) * 8) - 1; i >= 0; --i, ++j)
    {
        if (storage[i] != 0)
        {
            return bits - __builtin_clz(storage[i]) - j * 8 * sizeof(unsigned int);
        }
    }
}
template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator+(const Bigint &x) const
{
    Bigint<bits> res;
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
            if (k < bits / (sizeof(unsigned int) * 8))
            {
                temp = (unsigned long long)storage[i] * (unsigned long long)x.storage[j] + (unsigned long long)carry;
                carry = temp >> (8 * sizeof(unsigned int));
                res[k] += (unsigned int)temp;
            }
        }
        if (k < bits / (sizeof(unsigned int) * 8))
            res[k] = carry;
    }
    if (carry)
        throw("overflow");
    return res;
}
template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator/(const Bigint &x) const
{
    unsigned int bd(this->num_bits() - x.num_bits());
    Bigint rem(*this);
    Bigint quo(0);
    Bigint c(x << bd);
    Bigint e(Bigint(1) << bd);
    while (true)
    {
    }
}
template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator<<(const unsigned int &shift) const
{
    if (shift >= bits)
        return Bigint();
    Bigint ret;
    unsigned int full_shifts = shift / (sizeof(unsigned int) * 8);
    unsigned int lshift = shift % (sizeof(unsigned int) * 8);
    unsigned int rshift = sizeof(unsigned int) * 8 - lshift;
    if (lshift == 0)
    {
        for (unsigned int i = bits / (sizeof(unsigned int) * 8) - 1; i > full_shifts; --i)
            ret.storage[i] = storage[i - full_shifts] << lshift;
    }
    else
    {
        for (unsigned int i = bits / (sizeof(unsigned int) * 8) - 1; i > full_shifts; --i)
        {
            unsigned int lo = storage[i - full_shifts] << lshift;
            lo |= storage[i - full_shifts - 1] >> rshift;
            ret.storage[i] = lo;
        }
    }
    ret.storage[full_shifts] = storage[0] << lshift;
    return ret;
}
template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator>>(const unsigned int &shift) const
{
    if (shift >= bits)
        return Bigint();
    Bigint ret;
    unsigned int full_shifts = shift / (sizeof(unsigned int) * 8);
    unsigned int small_shift = shift & ((sizeof(unsigned int) * 8) - 1);
    unsigned int n = bits / (sizeof(unsigned int) * 8) - full_shifts;
    if (small_shift == 0)
    {
        for (unsigned int i = 0; i < n; ++i)
            ret.storage[i] = storage[i + full_shifts];
    }
    else
    {
        for (unsigned int i = 0; i < n - 1; ++i)
        {
            unsigned int lo = storage[i + full_shifts] >> small_shift;
            lo |= storage[i + full_shifts + 1] << (sizeof(unsigned int) * 8 - small_shift);
            ret.storage[i] = lo;
        }
        ret.storage[n - 1] = storage[n - 1 + full_shifts] >> small_shift;
    }
    return ret;
}
#endif
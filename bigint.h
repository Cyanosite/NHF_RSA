#ifndef BIGINT_H
#define BIGINT_H
#include "memtrace.h"
#include <iostream>
#include <iomanip>
#include <random>

/**
 * @tparam bits the number of bits used for storage.
 * If bits % 32 != 0: it will be rounded downwards to the nearest multiple of 32.
 */
template <unsigned int bits = 64>
struct Bigint
{
    // stores the number in a little-endian order
    unsigned int *storage;
    Bigint(const unsigned long long & = 0);
    //
    Bigint(const char *const &);
    Bigint(const Bigint &);
    Bigint &operator=(const Bigint &);
    // randomizes the number up to (input/32) bits
    void rng(const unsigned int & = 0);
    bool operator==(const Bigint &) const;
    bool operator!=(const Bigint &) const;
    bool operator<(const Bigint &) const;
    bool operator>(const Bigint &) const;
    // returns the number of bits used to represent the number
    unsigned int num_bits() const;
    bool is_even() const;
    bool is_odd() const;
    Bigint operator+(const Bigint &) const;
    Bigint operator-(const Bigint &) const;
    Bigint operator*(const Bigint &) const;
    Bigint operator/(const Bigint &) const;
    Bigint operator%(const Bigint &) const;
    Bigint operator<<(const unsigned int &) const;
    Bigint operator>>(const unsigned int &) const;
    ~Bigint();
};

template <unsigned int bits>
Bigint<bits>::Bigint(const unsigned long long &x)
{
    storage = new unsigned int[bits / (sizeof(unsigned int) * 8)]{0};
    storage[0] = x;
    storage[1] = x >> (sizeof(unsigned int) * 8);
}

/**
 * @param x C string with a '\0' null terminator.
 * It may only consist a hexadecimal number (letter capitalization doesn't matter).
 */
template <unsigned int bits>
Bigint<bits>::Bigint(const char *const &x)
{
    storage = new unsigned int[bits / (sizeof(unsigned int) * 8)]{0};
    unsigned short number_of_runs = strlen(x) / 8;
    // a run consists of reading 8 hexadecimal digits enough to fill 32 bits
    if (number_of_runs > 1)
    {
        const char *startpos = x + strlen(x) * sizeof(char) - 8 * sizeof(char);
        int i = 0;
        for (; i < number_of_runs; ++i)
        {
            sscanf(startpos, "%8X", &storage[i]);
            startpos -= 8;
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
    storage = new unsigned int[bits / (sizeof(unsigned int) * 8)];
    std::memcpy(storage, x.storage, bits / 8);
}

template <unsigned int bits>
Bigint<bits> &Bigint<bits>::operator=(const Bigint &x)
{
    if (this != &x)
    {
    delete[] storage;
    storage = new unsigned int[bits / (sizeof(unsigned int) * 8)];
    std::memcpy(storage, x.storage, bits / 8);
    }
    return *this;
}

/**
 * @param size_max upper limit of randomization in terms of bit size.
 * If (size_max = 0) => the entire size of its storage will be randomized
 * @return randomized output (size in bits = size_max/32) using std::random_device
 */
template <unsigned int bits>
void Bigint<bits>::rng(const unsigned int &size_max)
{
    std::random_device rd;
    if (size_max == 0)
    {
        unsigned int n = bits / (sizeof(unsigned int) * 8);
        for (unsigned short i = 0; i < n; ++i)
        {
            this->storage[i] = rd();
        }
    }
    else
    {
        unsigned int n = size_max / (sizeof(unsigned int) * 8);
        for (unsigned short i = 0; i < n; ++i)
        {
            this->storage[i] = rd();
        }
    }
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
bool Bigint<bits>::operator<(const Bigint &x) const
{
    for (int i = bits / (sizeof(unsigned int) * 8) - 1; i >= 0; --i)
    {
        if (storage[i] != 0 || x.storage[i] != 0)
        {
            return storage[i] < x.storage[i];
        }
    }
    return false;
}

template <unsigned int bits>
bool Bigint<bits>::operator>(const Bigint &x) const
{
    for (int i = bits / (sizeof(unsigned int) * 8) - 1; i >= 0; --i)
    {
        if (storage[i] != 0 || x.storage[i] != 0)
        {
            return storage[i] > x.storage[i];
        }
    }
    return false;
}

/**
 * @return the number of bits sufficient to represent *this
 */
template <unsigned int bits>
unsigned int Bigint<bits>::num_bits() const
{
    unsigned int i = bits / (sizeof(unsigned int) * 8) - 1;
    while (i > 0 && storage[i] == 0)
        --i;
    return storage[i] == 0 ? i * 8 * sizeof(unsigned int) : (i + 1) * 8 * sizeof(unsigned int) - __builtin_clz(storage[i]);
}

template <unsigned int bits>
bool Bigint<bits>::is_even() const
{
    return !(this->storage[0] & 1);
}

template <unsigned int bits>
bool Bigint<bits>::is_odd() const
{
    return this->storage[0] & 1;
}

template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator+(const Bigint &x) const
{
    Bigint res;
    unsigned int carry = 0;
    unsigned long long temp;
    for (unsigned int i = 0; i < bits / (sizeof(unsigned int) * 8); ++i)
    {
        temp = (unsigned long long)storage[i] + (unsigned long long)x.storage[i] + (unsigned long long)carry;
        // the next carry will be the temp shifted down by 32
        carry = temp >> (8 * sizeof(unsigned int));
        res.storage[i] = (unsigned int)temp;
    }
    return res;
}

template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator-(const Bigint &x) const
{
    Bigint res;
    unsigned int borrow = 0;
    unsigned long long temp;
    for (unsigned int i = 0; i < bits / (sizeof(unsigned int) * 8); ++i)
    {
        temp = (unsigned long long)storage[i] - ((unsigned long long)x.storage[i] + (unsigned long long)borrow);
        borrow = temp >> (8 * sizeof(unsigned long long) - 1);
        res.storage[i] = (unsigned int)temp;
    }
    return res;
}

template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator*(const Bigint &x) const
{
    Bigint res;
    unsigned long long carry = 0;
    unsigned short k = 0;
    unsigned long long temp;
    for (unsigned int i = 0; i < bits / (sizeof(unsigned int) * 8); ++i)
    {
        carry = 0;
        for (unsigned int j = 0; j < bits / (sizeof(unsigned int) * 8); ++j)
        {
            k = i + j;
            if (k < bits / (sizeof(unsigned int) * 8))
            {
                temp = (unsigned long long)res.storage[k] + (unsigned long long)storage[i] * (unsigned long long)x.storage[j] + (unsigned long long)carry;
                res.storage[k] = (unsigned int)temp;
                carry = temp >> (8 * sizeof(unsigned int));
            }
            else
                break;
        }
    }
    return res;
}

template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator/(const Bigint &x) const
{
    if (*this < x)
        return *this;
    unsigned int bd = this->num_bits() - x.num_bits();
    Bigint rem(*this);
    Bigint quo;
    Bigint c(x);
    Bigint e(1);
    c = c << bd;
    e = e << bd;
    unsigned int ms_part = bits / (sizeof(unsigned int) * 8) - 1;
    unsigned int bitsize_m_1 = (sizeof(unsigned int) * 8) - 1;
    Bigint r;
    while (true)
    {
        r = rem - c;
        unsigned int d = 1 - (r.storage[ms_part] >> bitsize_m_1);
        if (d != 0)
        {
            rem = r;
            r = quo + e;
            quo = r;
        }
        if (bd-- == 0)
            break;
        c = c >> 1;
        e = e >> 1;
    }
    return quo;
}

template <unsigned int bits>
Bigint<bits> Bigint<bits>::operator%(const Bigint &x) const
{
    if (*this < x)
        return *this;
    unsigned int bd = this->num_bits() - x.num_bits();
    Bigint rem(*this);
    Bigint c(x);
    c = c << bd;
    unsigned int ms_part = bits / (sizeof(unsigned int) * 8) - 1;
    unsigned int bitsize_m_1 = (sizeof(unsigned int) * 8) - 1;
    Bigint r;
    while (true)
    {
        r = rem - c;
        unsigned int d = r.storage[ms_part] >> bitsize_m_1;
        if (d == 0)
            rem = r;
        if (bd-- == 0)
            break;
        c = c >> 1;
    }
    return rem;
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

template <unsigned int bits>
Bigint<bits>::~Bigint()
{
    delete[] storage;
}

// iostream operators
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
            if (x.storage[i] != 0)
            {
                os << (unsigned int)x.storage[i];
                isnull = false;
            }
        }
        else
            os << std::setw(8) << std::setfill('0') << (unsigned int)x.storage[i];
    }
    if (isnull)
        os << 0;
    os << std::dec;
    return os;
}

#endif

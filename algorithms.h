#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "memtrace.h"
#include "bigint.h"

template <unsigned int bits>
Bigint<bits> euclidean(Bigint<bits> &m, Bigint<bits> &a)
{
    Bigint<bits> r;
    const Bigint<bits> null;
    while (true)
    {
        r = m % a;
        if (r == null)
        {
            std::cout << a << std::endl;
            return a;
        }
        m = a;
        a = r;
    }
}
template <unsigned int bits>
Bigint<bits> exponentiation(Bigint<bits> &m, Bigint<bits> &a, Bigint<bits> &b)
{
    Bigint<bits> c(1);
    const Bigint<bits> null;
    a = a % m;
    while (b != null)
    {
        if (!b.is_even())
            c = (c * a) % m;
        a = (a * a) % m;
        b = b >> 1;
    }
    return c;
}
template <unsigned int bits>
Bigint<bits> congruence(Bigint<bits> &a, Bigint<bits> &b, const Bigint<bits> &om)
{
    Bigint<bits> m(om);
    Bigint<bits> p;
    Bigint<bits> q(b);
    const Bigint<bits> null;
    while (true)
    {
        Bigint<bits> t(m / a);
        Bigint<bits> r = m % a;
        if (r == null)
            return q % om;
        Bigint<bits> c = (p - (t * q)) % om;
        m = a;
        a = r;
        p = q;
        q = c;
    }
}
template <unsigned int bits>
bool prime_check(Bigint<bits> &m)
{
    Bigint<bits> high(m - 1);
    const Bigint<bits> one(1);
    for (unsigned short k = 0; k < 100; ++k)
    {
        Bigint<bits> a;
        a.rng(high.num_bits());
        if (euclidean(m, a) != one)
            return false;
        std::cout << "hmm" << std::endl;
        if (exponentiation(m, a, high) != one)
            return false;
    }
    return true;
}

#endif

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "memtrace.h"
#include "bigint.h"

template <unsigned int bits>
Bigint<bits> euclidean(Bigint<bits> m, Bigint<bits> a)
{
    Bigint<bits> r;
    const Bigint<bits> null;
    while (true)
    {
        r = m % a;
        if (r == null)
            return a;
        m = a;
        a = r;
    }
}
template <unsigned int bits>
Bigint<bits> exponentiation(Bigint<bits> a, Bigint<bits> b, Bigint<bits> m)
{
    Bigint<bits> c(1);
    const Bigint<bits> null;
    while (b != null)
    {

        if (b.is_odd())
        {
            c = (c * a) % m;
        }
        a = (a * a) % m;
        b = b >> 1;
    }
    return c;
}
template <unsigned int bits>
Bigint<bits> inverse(Bigint<bits> a, Bigint<bits> b)
{
    Bigint<bits> b0(b);
    Bigint<bits> x0;
    bool x0_sign = false;
    Bigint<bits> x1(1);
    bool x1_sign = false;
    while (a > 1)
    {
        Bigint<bits> q(a / b);
        Bigint<bits> t = b;
        b = a % b;
        a = t;
        Bigint<bits> t2(x0);
        bool t2_sign = x0_sign;
        Bigint<bits> qx0(q * x0);
        if (x0_sign != x1_sign)
        {
            x0 = x1 + qx0;
            x0_sign = x1_sign;
        }
        else
        {
            x0 = (x1 > qx0) ? x1 - qx0 : qx0 - x1;
            x0_sign = x1 > qx0 ? x1_sign : !x0_sign;
        }
        x1 = t2;
        x1_sign = t2_sign;
    }
    return x1_sign ? b0 - x1 : x1;
}
template <unsigned int bits>
bool prime_check(Bigint<bits> m)
{
    Bigint<bits> high(m - 1);
    const Bigint<bits> one(1);
    for (unsigned short k = 0; k < 100; ++k)
    {
        Bigint<bits> a;
        a.rng(high.num_bits());
        if (euclidean(m, a) != one)
            return false;
        if (exponentiation(m, a, high) != one)
            return false;
    }
    return true;
}

#endif

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "bigint.h"
#include "memtrace.h"

/**
 * Euclidean algorithm finding the greatest common divisor of the given inputs.
 * @tparam bits number of bits used to store the integers, usually ommited in functions calls.
 * @param a
 * @return Greatest common divisor of a and b.
 */
template <unsigned int bits>
Bigint<bits> gcd(Bigint<bits> a, Bigint<bits> b)
{
    Bigint<bits> temp;
    Bigint<bits> null;
    while (b != null)
    {
        temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

/**
 * Modular exponentiation algorithm.
 * @tparam bits number of bits used to store the integers, usually ommited in functions calls.
 * @param a must be less than m
 * @param b must be greater than 0
 * @param m must be larger than a
 * @return aˆb % m
 */
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

/**
 * Modular multiplicative inverse algorithm (using extended Euclidean algorithm).
 * A modular multiplicative inverse of an integer a is an integer x such that the product ax is congruent to 1 with respect to the modulus m.
 * @tparam bits number of bits used to store the integers, usually ommited in functions calls.
 * @return a*t congruent 1 (mod b)
 */
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

/**
 * Fermat primality test algorithm.
 * @tparam bits number of bits used to store the integers, usually ommited in functions calls.
 * @param m the integer to test
 * @return True if m is a prime.
 */
template <unsigned int bits>
bool prime_check(Bigint<bits> m)
{
    Bigint<bits> high(m - 1);
    const Bigint<bits> one(1);
    for (unsigned short k = 0; k < 100; ++k)
    {
        Bigint<bits> a;
        a.rng(high.num_bits());
        if (gcd(m, a) != one)
            return false;
        if (exponentiation(a, high, m) != one)
            return false;
    }
    return true;
}

#endif

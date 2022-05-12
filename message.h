#ifndef MESSAGE_H
#define MESSAGE_H

#include "memtrace.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "algorithms.h"
#include "bigint.h"

//#define DEBUG

enum message_size
{
    bigint_size = 256,
    key_size = 128,
    prime_size = 64,
    c_size = 32,
};
class Message
{
    std::vector<Bigint<bigint_size> > message;
    Bigint<bigint_size> primes[2];
    Bigint<bigint_size> c;
    Bigint<bigint_size> public_key;
    Bigint<bigint_size> private_key;
    bool is_encrypted;

public:
    Message() : is_encrypted(false) {}
    Message(const char *text) : is_encrypted(false)
    {
        message.resize(strlen(text));
        std::copy(text, text + strlen(text), message.begin());
    }
    Message(std::string &string) : is_encrypted(false)
    {
        message.resize(string.length());
        std::copy(string.begin(), string.end(), message.begin());
    }
    // for testing purposes
    bool operator==(const Message &x) const
    {
        return message == x.message;
    }
    void encrypt()
    {
        for (unsigned short i = 0; i < 2; ++i)
        {
            Bigint<bigint_size> my_prime;
            do
            {
                my_prime.rng(prime_size);
            } while (!prime_check(my_prime));
#ifdef DEBUG
            std::cout << "prime found: " << my_prime << std::endl;
#endif
            primes[i] = my_prime;
        }
        public_key = primes[0] * primes[1];
        Bigint<bigint_size> null;
        Bigint<bigint_size> one(1);
        do
        {
            c.rng(c_size);
        } while (!prime_check(c) || gcd(public_key, c) != one);
#ifdef DEBUG
        std::cout << "c: " << c << std::endl;
        std::cout << "public key: " << public_key << std::endl;
#endif
        for (std::vector<Bigint<bigint_size> >::iterator i = message.begin(); i < message.end();)
            *i++ = exponentiation(*i, c, public_key);
        is_encrypted = true;
    }
    void decrypt()
    {
        Bigint<bigint_size> one(1);
        Bigint<bigint_size> temp1(primes[0] - one);
        Bigint<bigint_size> temp2(primes[1] - one);
        private_key = (temp1 * temp2) / gcd(temp1, temp2);
        Bigint<bigint_size> decryption_key = inverse(c, private_key);
#ifdef DEBUG
        std::cout << "gcd: " << gcd(temp1, temp2) << std::endl;
        std::cout << "private_key: " << private_key << std::endl;
        std::cout << "decryption key: " << decryption_key << std::endl;
#endif
        for (std::vector<Bigint<bigint_size> >::iterator i = message.begin(); i < message.end();)
            *i++ = exponentiation(*i, decryption_key, public_key);
        is_encrypted = false;
    }
    friend std::ostream &operator<<(std::ostream &, Message &);
};
std::ostream &operator<<(std::ostream &os, Message &x)
{
    for (std::vector<Bigint<bigint_size> >::iterator i = x.message.begin(); i < x.message.end(); ++i)
        os << (char)(i->storage[0]);
    return os;
}

#endif
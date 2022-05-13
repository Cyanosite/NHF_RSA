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
    std::vector<Bigint<bigint_size>> message;
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

    Message(const std::string &string) : is_encrypted(false)
    {
        message.resize(string.length());
        std::copy(string.begin(), string.end(), message.begin());
    }

    Message(const Message &x) : message(x.message), primes(x.primes), c(x.c), public_key(x.public_key), private_key(x.private_key), is_encrypted(x.is_encrypted) {}
    Message &operator=(const Message &x)
    {
        if (this != &x)
        {
            message = x.message;
            primes[0] = x.primes[0];
            primes[1] = x.primes[1];
            c = x.c;
            public_key = x.public_key;
            private_key = x.private_key;
            is_encrypted = x.is_encrypted;
        }
    }

    /**
     * @param index the position of the character
     * @return the character at the index, it's not a reference as this
     * should not be changed from the outside directly
     */
    char operator[](const unsigned short &index) const
    {
        return (char)message[index].storage[0];
    }

    Message operator+(const char &x) const
    {
        if (is_encrypted)
            throw("Cannot extend encrypted message, please decrypt it first");
        Message res(*this);
        res.message.push_back(x);
        return res;
    }

    Message operator+(const std::string &string) const
    {
        if (is_encrypted)
            throw("Cannot extend encrypted message, please decrypt it first");
        Message res(*this);
        size_t message_size = message.size();
        res.message.resize(message_size + string.length());
        std::copy(string.begin(), string.end(), res.message.at(message_size));
        return res;
    }

    Message operator+(const Message &x) const
    {
        if (is_encrypted)
            throw("Cannot extend encrypted message, please decrypt it first");
        Message res(*this);
        size_t message_size = message.size();
        res.message.resize(message_size + x.message.size());
        std::copy(x.message.begin(), x.message.end(), res.message.at(message_size));
        return res;
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
        for (std::vector<Bigint<bigint_size>>::iterator i = message.begin(); i < message.end();)
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
        for (std::vector<Bigint<bigint_size>>::iterator i = message.begin(); i < message.end();)
            *i++ = exponentiation(*i, decryption_key, public_key);
        is_encrypted = false;
    }
    friend std::ostream &operator<<(std::ostream &, Message &);
};

std::ostream &operator<<(std::ostream &os, Message &x)
{
    for (std::vector<Bigint<bigint_size>>::iterator i = x.message.begin(); i < x.message.end(); ++i)
        os << (char)(i->storage[0]);
    return os;
}

#endif

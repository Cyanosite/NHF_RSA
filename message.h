#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "bigint.h"
#include "memtrace.h"

//#define DEBUG

/**
 * Defines the bit-width of the bigints stored in the message, the key_size of the rsa algorithm
 * and the primes used for the key creation.
 */
enum message_size
{
    // bigint_size has to be twice the size of key_size as we need to account for multiplication inside the algorithms
    bigint_size = 256,
    // key_size has to be twice the size of the primes as this is created through their multiplication
    key_size = 128,
    // we use the value of prime_size to determine the values of the above two
    prime_size = 64,
    // c_size can always stay 32 bits, it just has to be smaller than the primes
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

    Message(const std::string &string) : is_encrypted(false)
    {
        message.resize(string.length());
        std::copy(string.begin(), string.end(), message.begin());
    }

    Message(const Message &x) : message(x.message), c(x.c), public_key(x.public_key), private_key(x.private_key), is_encrypted(x.is_encrypted)
    {
        primes[0] = x.primes[0];
        primes[1] = x.primes[1];
    }
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
        return *this;
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
            throw(std::logic_error("Cannot concatenate encrypted message, please decrypt it first"));
        Message res(*this);
        res.message.push_back(x);
        return res;
    }

    Message operator+(const std::string &string) const
    {
        if (is_encrypted)
            throw(std::logic_error("Cannot concatenate encrypted message, please decrypt it first"));
        Message res(*this);
        size_t message_size = message.size();
        res.message.resize(message_size + string.length());
        std::copy(string.begin(), string.end(), res.message.begin() + message_size);
        return res;
    }

    Message operator+(const Message &x) const
    {
        if (is_encrypted)
            throw(std::logic_error("Cannot concatenate encrypted message, please decrypt it first"));
        Message res(*this);
        size_t message_size = message.size();
        res.message.resize(message_size + x.message.size());
        std::copy(x.message.begin(), x.message.end(), res.message.begin() + message_size);
        return res;
    }

    // for testing purposes
    bool operator==(const Message &x) const
    {
        return message == x.message;
    }

    void encrypt()
    {
        // generate the 2 primes for the algorithm
        for (unsigned short i = 0; i < 2; ++i)
        {
            // we generate a new number until we find a prime
            Bigint<bigint_size> my_prime;
            do
            {
                my_prime.rng(prime_size);
            } while (!my_prime.prime_check());
#ifdef DEBUG
            std::cout << "prime found: " << my_prime << std::endl;
#endif
            primes[i] = my_prime;
        }
        // the public key will be the product of the primes
        public_key = primes[0] * primes[1];
        Bigint<bigint_size> null;
        Bigint<bigint_size> one(1);
        // find a c such that c and the public_key are coprimes
        do
        {
            c.rng(c_size);
        } while (!c.prime_check() || public_key.gcd(c) != one);
#ifdef DEBUG
        std::cout << "c: " << c << std::endl;
        std::cout << "public key: " << public_key << std::endl;
#endif
        // execute the encryption function on the entire message
        for (std::vector<Bigint<bigint_size> >::iterator i = message.begin(); i < message.end();)
            *i++ = (*i).exponentiation(c, public_key);
        is_encrypted = true;
    }
    void decrypt()
    {
        // calculate the private_key, this is the Carmichael's totient function
        // here the 2 inputs are the primes - 1 (the result of Euler's function, how many coprimes a given number has below them)
        // then we have to calculate the lcm of the primes - 1
        // we can do this as |a*b|/gcd(a,b)
        Bigint<bigint_size> one(1);
        Bigint<bigint_size> temp1(primes[0] - one);
        Bigint<bigint_size> temp2(primes[1] - one);
        private_key = (temp1 * temp2) / temp1.gcd(temp2);
        // determine the decryption key by getting the modular multiplicative inverse of c
        Bigint<bigint_size> decryption_key = c.inverse(private_key);
#ifdef DEBUG
        std::cout << "gcd: " << temp1.gcd(temp2) << std::endl;
        std::cout << "private_key: " << private_key << std::endl;
        std::cout << "decryption key: " << decryption_key << std::endl;
#endif
        // after finding the decryption key we just have to execute the decryption function
        for (std::vector<Bigint<bigint_size> >::iterator i = message.begin(); i < message.end();)
            *i++ = (*i).exponentiation(decryption_key, public_key);
        is_encrypted = false;
    }
    friend std::ostream &operator<<(std::ostream &, Message &);
};

std::ostream &operator<<(std::ostream &os, Message &x)
{
    // we print the LSB of the stored message converted to a character
    for (std::vector<Bigint<bigint_size> >::iterator i = x.message.begin(); i < x.message.end(); ++i)
        os << (char)(i->storage[0]);
    return os;
}

#endif

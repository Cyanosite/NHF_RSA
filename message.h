#ifndef MESSAGE_H
#define MESSAGE_H

#include "memtrace.h"
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include "algorithms.h"
#include "bigint.h"
class Message
{
    std::vector<Bigint<2048> > message;
    bool is_encrypted;
    Bigint<2048> public_key;
    Bigint<2048> primes[2];
    Bigint<2048> private_key;

public:
    Message() : message(), is_encrypted(false), primes() {}
    Message(const char *text) : message(), is_encrypted(false), primes()
    {
        for (unsigned short i = 0; i < strlen(text); ++i)
        {
            message.push_back(text[i]);
        }
    }
    void encrypt()
    {
        Bigint<2048> one(1);
        Bigint<2048> e(3);
        for (unsigned short i = 0; i < 2; ++i)
        {
            Bigint<2048> prime;
            do
            {
                prime.rng(32);
            } while (!prime_check(prime));
            std::cout << "prime found: " << prime << std::endl;
            primes[i] = prime;
        }
        public_key = primes[0] * primes[1];
        private_key = (primes[0] - one) * (primes[1] - one);
        std::cout << "public key: " << public_key << std::endl;
        for (std::vector<Bigint<2048> >::iterator i = message.begin(); i < message.end(); ++i)
        {
            *i = exponentiation(*i, e, public_key);
        }
        is_encrypted = true;
    }
    void decrypt()
    {
        Bigint<2048> e(3);
        Bigint<2048> decryption_key(inverse(e, private_key));
        std::cout << "decryption key = " << decryption_key << std::endl;
        for (std::vector<Bigint<2048> >::iterator i = message.begin(); i < message.end(); ++i)
        {
            *i = exponentiation(*i, decryption_key, public_key);
        }
    }
    friend std::ostream &operator<<(std::ostream &, Message &);
    friend std::istream &operator>>(std::istream &, Message &);
};
std::ostream &operator<<(std::ostream &os, Message &x)
{
    for (std::vector<Bigint<2048> >::iterator i = x.message.begin(); i < x.message.end(); ++i)
    {
        os << *i << ",";
    }
    return os;
}
std::istream &operator>>(std::istream &is, Message &x)
{
    std::string word;
    is >> word;
    x.message.clear();
    for (std::string::iterator i = word.begin(); i < word.end(); ++i)
    {
        x.message.push_back(*i);
    }
    return is;
}

#endif
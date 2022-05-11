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
    Bigint<2048> c;
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
        for (unsigned short i = 0; i < 2; ++i)
        {
            Bigint<2048> my_prime;
            do
            {
                my_prime.rng(64);
            } while (!prime_check(my_prime));
            std::cout << "prime found: " << my_prime << std::endl;
            primes[i] = my_prime;
        }
        public_key = primes[0] * primes[1];
        Bigint<2048> null;
        do
        {
            c.rng(63);
        } while (!prime_check(c) || public_key % c == null);
        std::cout << "c: " << c << std::endl;

        std::cout << "public key: " << public_key << std::endl;
        for (int i = 0; i < message.size(); ++i)
            message[i] = exponentiation(message[i], c, public_key);
    }
    void decrypt()
    {
        Bigint<2048> one(1);
        private_key = (primes[0] - one) * (primes[1] - one);
        std::cout << "private_key: " << private_key << std::endl;
        Bigint<2048> decryption_key = inverse(c, private_key);
        std::cout << "decryption key: " << decryption_key << std::endl;
        for (int i = 0; i < message.size(); ++i)
            message[i] = exponentiation(message[i], decryption_key, public_key);
    }
    friend std::ostream &operator<<(std::ostream &, Message &);
    friend std::istream &operator>>(std::istream &, Message &);
};
std::ostream &operator<<(std::ostream &os, Message &x)
{
    for (int i = 0; i < x.message.size(); ++i)
        os << x.message[i][0] << ", ";
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
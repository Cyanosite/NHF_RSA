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
    std::vector<Bigint<4096>> message;
    bool is_encrypted;
    Bigint<4096> public_key;
    Bigint<4096> private_keys[2];

public:
    Message() : is_encrypted(false), public_key(), private_keys() {}
    Message(const char *text) : message(), is_encrypted(false), public_key(), private_keys()
    {
        for (unsigned short i = 0; i < strlen(text); ++i)
        {
            message.push_back(text[i]);
        }
    }
    void encrypt()
    {
        Bigint<4096> e(65537);
        for (unsigned short i = 0; i < 2; ++i)
        {
            Bigint<4096> prime;
            do
            {
                prime.rng(2048);
                std::cout << prime << std::endl;
            } while (!prime_check(prime));
            private_keys[i] = prime;
        }
        std::cout << private_keys[0] << "\n"
                  << private_keys[1] << std::endl;
        public_key = private_keys[0] * private_keys[1];
        for (std::vector<Bigint<4096>>::iterator i = message.begin(); i < message.end(); ++i)
        {
            *i = exponentiation(public_key, *i, e);
        }
    }
    void decrypt()
    {
        Bigint<4096> e(65537);
        Bigint<4096> one(1);
        Bigint<4096> decryption_key = congruence(e, one, (private_keys[0] - 1) * (private_keys[1] - 1));
        for (std::vector<Bigint<4096>>::iterator i = message.begin(); i < message.end(); ++i)
        {
            *i = exponentiation(public_key, *i, decryption_key);
        }
    }
    friend std::ostream &operator<<(std::ostream &, Message &);
    friend std::istream &operator>>(std::istream &, Message &);
};
std::ostream &operator<<(std::ostream &os, Message &x)
{
    for (std::vector<Bigint<4096>>::iterator i = x.message.begin(); i < x.message.end(); ++i)
    {
        os << (char)i->storage[0];
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
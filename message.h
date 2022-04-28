#ifndef MESSAGE_H
#define MESSAGE_H

#include <iostream>
#include <string>
#include "bigint.h"

class Message
{
    std::string message;
    bool is_encrypted;
    Bigint<4096> public_key;
    Bigint<4096> private_keys[2];

public:
    Message() : is_encrypted(false), public_key(), private_keys() {}
    Message(const std::string &text) : message(text), is_encrypted(false), public_key(), private_keys() {}
    Message(const char *text) : message(text), is_encrypted(false), public_key(), private_keys() {}
    void encrypt();
    void decrypt();
};
std::ostream &operator<<(std::ostream &, const Message &);
std::istream &operator>>(std::istream &, Message &);
#endif
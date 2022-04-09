#include <ctime>
#include <iostream>
#include "memtrace.h"
#include "bigint.h"
int main()
{
    Bigint<2048> bignumber("123456789ABCDEF123456789ABCDEF");
    Bigint<2048> big_copy(bignumber);
    Bigint<2048> big_copy2(bignumber);
    bignumber = bignumber + big_copy + big_copy2 + big_copy;
    std::cout << bignumber << std::endl;
    Bigint<2048> sub = bignumber - bignumber;
    std::cout << sub << std::endl;
    Bigint<2048> mul = big_copy * Bigint<2048>(4);
    std::cout << mul << std::endl;
    if (bignumber == mul)
        std::cout << "bignumber and mul are equal" << std::endl;
    /*Bigint<256> N("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
    Bigint<256> EIGHTY_EIGHT("FFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD03641410000000000000000000000");
    if ((N << 88) == EIGHTY_EIGHT)
        std::cout << "<< works lmao, how?" << std::endl;*/
    Bigint<256> N2("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
    Bigint<256> N3("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5D576E7357A4501DDFE92F46681B20A0");
    std::cout << N3 << " == " << (N2 >> 1) << std::endl;
    if ((N2 >> 1) == N3)
        std::cout << "<< works lmao, how?" << std::endl;
    return 0;
}

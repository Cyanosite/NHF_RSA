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
    Bigint<256> N2("FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141");
    Bigint<256> N3("7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF5D576E7357A4501DDFE92F46681B20A0");
    std::cout << N3 << " == " << (N2 >> 1) << std::endl;
    if ((N2 >> 1) == N3)
        std::cout << ">> works lmao, how?" << std::endl;
    if ((N2 >> 1) << 1 == N2)
        std::cout << "<< works lmao, how?" << std::endl;
    Bigint<1024> ASD("00000000000FFFF23461356813480000000000");
    if (ASD << 13 >> 13 == ASD)
        std::cout << "ppoggersdn lsdfjkgnwsdfgkljhnlfgdhn" << std::endl;
    return 0;
}

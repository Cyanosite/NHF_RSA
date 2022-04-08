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
    return 0;
}

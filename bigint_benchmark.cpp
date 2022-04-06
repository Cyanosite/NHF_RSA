#include <ctime>
#include "memtrace.h"
#include "bigint.h"
int main()
{
    Bigint<2048>("FFFFFFFFFFFFFFFF").print();
    std::cout << std::endl;
    Bigint<>(18446744073709551615).print();
    return 0;
}

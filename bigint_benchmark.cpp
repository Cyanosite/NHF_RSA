#include <ctime>
#include "memtrace.h"
#include "bigint.h"
int main()
{
    Bigint<2048>("423abc52a").print();
    Bigint<>(235621).print();
    return 0;
}
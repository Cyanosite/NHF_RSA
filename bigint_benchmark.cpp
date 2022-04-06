#include <ctime>
#include "memtrace.h"
#include "bigint.h"
int main()
{
    Bigint<2048> asd("FFFFFFFFFFFFFFFF");
    Bigint<2048> asd2(asd);
    Bigint<2048> asd3(asd);
    asd = asd + asd2 + asd3;
    asd.print();
    return 0;
}

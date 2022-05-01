#include "gtest_lite.h"
#include "bigint.h"

int main()
{
    TEST(Constructor from integer, x = ABCDEF98)
    {
        Bigint<128> x(0xABCDEF98);
        EXPECT_EQ(x[0], 0xABCDEF98U) << "ctr from int failed";
    }
    END
    TEST(Constructor from string, x = 123456789ABCDEF1)
    {
        Bigint<128> x("123456789ABCDEF1");
        EXPECT_EQ(x[1], 0x12345678U) << "ctr from string failed";
        EXPECT_EQ(x[0], 0x9ABCDEF1U) << "ctr from string failed";
    }
    END
    // on copy, the storage pointers should equal as only the clone method makes a new array
    // TODO: MAKE IT WORK THIS WAY
    /*TEST(Copy constructor, x = 123456789ABCDEF1)
    {
        Bigint<128> x("123456789ABCDEF1");
        EXPECT_EQ(x[1], 0x12345678U) << "ctr from string failed";
        EXPECT_EQ(x[0], 0x9ABCDEF1U) << "ctr from string failed";
    }
    END*/
    TEST(How many bits to represent, x = 123456789ABCDEF123456789ABCDEF)
    {
        Bigint<256> x("123456789ABCDEF123456789ABCDEF");
        EXPECT_EQ(x.num_bits(), 117U) << "bit counting failed"; // 30 * 4 - 3
    }
    END

    TEST(X is even, x = 123476)
    {
        Bigint<> x(123476);
        EXPECT_EQ(x.is_even(), true) << "x should be even";
    }
    END
    TEST(X is odd, x = 982347)
    {
        Bigint<> x(982347);
        EXPECT_EQ(x.is_odd(), true) << "x should be odd";
    }
    END
    TEST(Operation, addition)
    {
        Bigint<256> x("23497ab638923c8934dfe231988");
        Bigint<256> y("bcd52348edf0909349819d8c881391812b");
        Bigint<256> res("bcd5234b22883bf6d2a5661fd611b49ab3");
        EXPECT_EQ(x + y, res) << "addition failed";
    }
    END
    TEST(Operation, subtraction)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> y("23497ab638923c8934dfe231988");
        Bigint<256> res("bcd52346b958e52fc05dd4f93a156e67a3");
        EXPECT_EQ(x - y, res) << "subtraction failed";
    }
    END
    TEST(Operation, multiplication)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> y("23497ab638923c8934dfe231988");
        Bigint<256> res("1a07571e0466128867f36489d9e7dc1f3ed243873ce31b2f8882fedcad1d8");
        EXPECT_EQ(x * y, res) << "multiplication failed";
    }
    END
    TEST(Operation, division)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> y("23497ab638923c8934dfe231988");
        Bigint<256> res("559f0d3a");
        EXPECT_EQ(x / y, res) << "division failed";
    }
    END
    TEST(Operation, modulo)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> y("23497ab638923c8934dfe231988");
        Bigint<256> res("1d0849f3ce9d3deb7adb3d9d05b");
        EXPECT_EQ(x % y, res) << "modulo failed";
    }
    END
    TEST(Operation, left shift)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> res("2f3548d23b7c2424d26067632204e4604ac00000000000000000");
        EXPECT_EQ(x << 70, res) << "left shift failed";
    }
    END
    TEST(Operation, right shift)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> res("2f3548d23b7c2424d");
        EXPECT_EQ(x >> 70, res) << "right shift failed";
    }
    END return 0;
}

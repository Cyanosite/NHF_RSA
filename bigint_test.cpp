#include "memtrace.h"
#include "gtest_lite.h"
#include "bigint.h"
#include "message.h"
#include <iostream>

int main()
{
    TEST(Constructor from integer, x = ABCDEF98)
    {
        Bigint<128> x(0xABCDEF98);
        EXPECT_EQ(0xABCDEF98U, x.storage[0]) << "ctr from int failed";
    }
    END
    TEST(Constructor from string, x = 123456789ABCDEF1)
    {
        Bigint<128> x("123456789ABCDEF1");
        EXPECT_EQ(0x12345678U, x.storage[1]) << "ctr from string failed";
        EXPECT_EQ(0x9ABCDEF1U, x.storage[0]) << "ctr from string failed";
    }
    END
    TEST(How many bits to represent, x = 123456789ABCDEF123456789ABCDEF)
    {
        Bigint<256> x("123456789ABCDEF123456789ABCDEF");
        EXPECT_EQ(117U, x.num_bits()) << "bit counting failed"; // 30 * 4 - 3
    }
    END

    TEST(X is even, x = 123476)
    {
        Bigint<> x(123476);
        EXPECT_EQ(true, x.is_even()) << "x should be even";
    }
    END
    TEST(X is odd, x = 982347)
    {
        Bigint<> x(982347);
        EXPECT_EQ(true, x.is_odd()) << "x should be odd";
    }
    END
    TEST(Operation, addition)
    {
        Bigint<256> x("23497ab638923c8934dfe231988");
        Bigint<256> y("bcd52348edf0909349819d8c881391812b");
        Bigint<256> res("bcd5234b22883bf6d2a5661fd611b49ab3");
        EXPECT_EQ(res, x + y) << "addition failed";
    }
    END
    TEST(Operation, addition_overflow)
    {
        unsigned long long int x = 0xFFFFFFFFFFFFFFFF;
        Bigint<64> y("FFFFFFFFFFFFFFFF");
        Bigint<64> one(1);
        Bigint<64> res(x + 1);
        EXPECT_EQ(res, y + one) << "addition_overflow failed";
    }
    END
    TEST(Operation, subtraction)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> y("23497ab638923c8934dfe231988");
        Bigint<256> res("bcd52346b958e52fc05dd4f93a156e67a3");
        EXPECT_EQ(res, x - y) << "subtraction failed";
    }
    END
    TEST(Operation, subtraction_underflow)
    {

        unsigned long long int x = 0;
        Bigint<64> y;
        Bigint<64> one(1);
        Bigint<64> res(x - 1);
        EXPECT_EQ(res, y - one) << "subtraction_underflow failed";
    }
    END
    TEST(Operation, multiplication)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> y("23497ab638923c8934dfe231988");
        Bigint<256> res("1a07571e0466128867f36489d9e7dc1f3ed243873ce31b2f8882fedcad1d8");
        EXPECT_EQ(res, x * y) << "multiplication failed";
    }
    END
    TEST(Operation, division)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> y("23497ab638923c8934dfe231988");
        Bigint<256> res("559f0d3a");
        EXPECT_EQ(res, x / y) << "division failed";
    }
    END
    TEST(Operation, modulo)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> y("23497ab638923c8934dfe231988");
        Bigint<256> res("1d0849f3ce9d3deb7adb3d9d05b");
        EXPECT_EQ(res, x % y) << "modulo failed";
    }
    END
    TEST(Operation, left shift)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> res("2f3548d23b7c2424d26067632204e4604ac00000000000000000");
        EXPECT_EQ(res, x << 70) << "left shift failed";
    }
    END
    TEST(Operation, right shift)
    {
        Bigint<256> x("bcd52348edf0909349819d8c881391812b");
        Bigint<256> res("2f3548d23b7c2424d");
        EXPECT_EQ(res, x >> 70) << "right shift failed";
    }
    END
    TEST(Algorithm, gcd)
    {
        Bigint<> m(1238);
        Bigint<> a(567);
        Bigint<> one(1);
        EXPECT_EQ(one, gcd(m, a)) << "euclidean failed";
    }
    END
    TEST(Algorithm, gcd 2)
    {
        Bigint<256> m("84fb24809d95c5315fb061d16fe016c0e55bd085da79a7c3cda921b3f3c364");
        Bigint<256> a("3a283606d4325f6a8fdedec81289481c8");
        Bigint<256> four(4);
        EXPECT_EQ(four, gcd(m, a)) << "euclidean 2 failed";
    }
    END
    TEST(Algorithm, exponentiation)
    {
        Bigint<256> a(13);
        Bigint<256> b(53);
        Bigint<256> m(97);
        Bigint<256> result(23);
        EXPECT_EQ(result, exponentiation(a, b, m)) << "exponentiation failed";
    }
    END
    TEST(Algorithm, exponentiation 2)
    {
        Bigint<256> a("2fc49c36f3759e607989819908be7c08");
        Bigint<256> b("944dea746e003341508a6b4b");
        Bigint<256> m("81dad55da5b9126e9f");
        Bigint<256> result("754c14c8901dc84ec2");
        EXPECT_EQ(result, exponentiation(a, b, m)) << "exponentiation 2 failed";
    }
    END
    TEST(Algorithm, inverse)
    {
        Bigint<1024> a("2481f32ab7fe49d59fd6e336aa4c1c53ddc985f2d6d9dd");
        Bigint<1024> m("e53fa29f5cd9f1cda165049bfe4b8187e0cb75fe6730c37871788ea57aee4841d0fabb387a1ca85dc2");
        Bigint<1024> result("945fea503a3d2f1f8b9fb6c60ab407c5003923d93c7dab6f5bafee46b81f70d32e4c0ea7d3bef78e03");
        EXPECT_EQ(result, inverse(a, m)) << "inverse failed";
    }
    END
    TEST(Algorithm, inverse 2)
    {
        Bigint<1024> e("865d98df4f0be16466b1");
        Bigint<1024> m("1ae09926bc4aec40ab4e8916c56f023fb92b");
        Bigint<1024> result("d133bc208ff54618ad91d792f46a4b31957");
        EXPECT_EQ(result, inverse(e, m)) << "inverse 2 failed";
    }
    END
    TEST(RSA, encryption and decryption)
    {
        Message equal("Hello World");
        std::string my_string("Hello World");
        Message my_message(my_string);
        std::cout << my_message << std::endl;
        my_message.encrypt();
        std::cout << my_message << std::endl;
        my_message.decrypt();
        std::cout << my_message << std::endl;
        EXPECT_EQ(equal, my_message);
    }
    END return 0;
}

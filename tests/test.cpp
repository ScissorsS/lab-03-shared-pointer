// Copyright 2018 Your Name <your_email>

#include <gtest/gtest.h>
#include "header.hpp"

TEST(Example, EmptyTest) {
char *test_ptr1 = new char;
*test_ptr1 = 'a';

SharedPtr<char> pointer1(test_ptr1);
EXPECT_EQ(pointer1.get(), test_ptr1);
EXPECT_EQ(pointer1.use_count(), 1);

SharedPtr<char> pointer2(pointer1);
EXPECT_EQ(pointer2.get(), pointer1.get());
EXPECT_EQ(pointer1.use_count(), 2);
EXPECT_EQ(pointer2.use_count(), 2);

char *test_ptr2 = new char;
*test_ptr2 = 'b';
SharedPtr<char> pointer3(test_ptr2);
EXPECT_EQ(*pointer3, *test_ptr2);

pointer2 = pointer3;
EXPECT_EQ(pointer2.get(), pointer3.get());

pointer1.Swap(pointer3);
EXPECT_EQ(pointer1.get(), test_ptr2);
EXPECT_EQ(pointer3.get(), test_ptr1);

pointer3.reset();
EXPECT_EQ(pointer3.use_count(), 0);
EXPECT_EQ(pointer3.get(), nullptr);

pointer2.reset(test_ptr2);
EXPECT_EQ(pointer2.get(), nullptr);
EXPECT_EQ(pointer1.use_count(), 0);
EXPECT_EQ(pointer2.use_count(), 0);
delete test_ptr2;
pointer1.reset();
pointer2.reset();
}
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*************************************************************************
	> File Name: main.cpp
	> Author: 
	> Mail: 
	> Created Time: Mon Sep 13 08:41:06 2021
 ************************************************************************/

#include<iostream>
#include "./z_jd_test.h"
using namespace std;

int add(int a, int b) {
	return a + b;
}

TEST(test1, add_function) {
    EXPECT_EQ(add(3, 4), 7); // ==
    EXPECT_NE(add(3, 4), 6); // !=
    EXPECT_LT(add(3, 4), 8); // <
    EXPECT_LE(add(3, 4), 7); // <=
    EXPECT_GT(add(3, 4), 6); // >
    EXPECT_GE(add(3, 4), 7); // >=
}

TEST(test2, add_function) {
    EXPECT_EQ(add(3, 4), 6); // ==
    EXPECT_NE(add(3, 4), 7); // !=
    EXPECT_LT(add(3, 4), 6); // <
    EXPECT_LE(add(3, 4), 6); // <=
    EXPECT_GT(add(3, 4), 8); // >
    EXPECT_GE(add(3, 4), 8); // >=
}

int main() {
	RUN_ALL_TESTS();
	return 0;
}
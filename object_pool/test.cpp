#include "pool.h"

#include <iostream>
#include <gtest/gtest.h>
#include <string>
#include <utility>

#include <crtdbg.h> 

#include "testclass.h"


bool operator == (const point& p1, const point&p2) {
	return(p1.x == p2.x && p1.y == p2.y);
}

class PoolTesting : public ::testing::Test {
public:
	virtual void SetUp(void) {
		_CrtMemCheckpoint(&startup);
	}
	virtual void TearDown(void) {
		_CrtMemState teardown, diff;
		_CrtMemCheckpoint(&teardown);
		ASSERT_EQ(0, _CrtMemDifference(&diff, &startup, &teardown)) << "Memory leaks detected";
	}
	_CrtMemState startup;
};

TEST_F(PoolTesting, CharAsterTest) {
	pool<const char*> charastpool(10);
	for (size_t i = 0; i < 10; ++i) {
		charastpool.alloc("gsg");
	}
	for (size_t i = 5; i < 9; ++i) {
		charastpool.free(&charastpool[i]);
	}
	for (size_t i = 0; i < 4; ++i) {
		charastpool.alloc("gsg");
	}
	for (size_t i = 0; i < 10; ++i) {
		charastpool.free(&charastpool[i]);
	}
}

TEST_F(PoolTesting, ArrayTest) {
	pool<int[3]> arpool(10);
	int a[3]{ 1, 2, 3 };
	for (size_t i = 0; i < 10; ++i) {
		arpool.alloc(&a);
	}
}

TEST_F(PoolTesting, ArrayTestEmptyAlloc) {
	pool<int[3]> arpool(10);
	int a[3]{ 1, 2, 3 };
	for (size_t i = 0; i < 10; ++i) {
		arpool.alloc();
	}
}

TEST_F(PoolTesting, StupidStringTest) {
	pool<stupidString> sspool(10); 
	char a[] = "abc", b[] = "bcd", c[] = "cde", d[] = "def";
	stupidString * pa[10];
	for (size_t i = 0; i < 10; ++i) {
		ASSERT_NO_THROW(pa[i] = sspool.alloc(a));
	}
	for (size_t i = 0; i < 10; ++i) {
		ASSERT_NO_THROW(sspool.free(pa[i]));
		pa[i] = nullptr;
	}
	stupidString to_be_moved(a);
	stupidString& reference_to_that_which_is_to_be_moved = to_be_moved;
	sspool.alloc(stupidString(a));
	sspool.alloc(to_be_moved);
	sspool.alloc(reference_to_that_which_is_to_be_moved);
	sspool.alloc(std::move(to_be_moved));

}

TEST_F(PoolTesting, StdStringTest) {
	pool<std::string> sspool(10); 
	std::string str_a{ "abc" };
	for (size_t i = 0; i < 10; ++i) {
		sspool.alloc(str_a);
	}

}

TEST_F(PoolTesting, Point) {
	pool<point> pointpool(10);
	point temp;
	point* pi = &temp;
	for (size_t i = 0; i < 10; ++i) {
		pi = pointpool.alloc();
		*pi = point(i, i);
		ASSERT_EQ(*pi, point(i, i));
	}
	for (size_t i = 0; i < 10; ++i) {
		ASSERT_NO_THROW(pointpool.free(pi - i));
	}
	for (size_t i = 0; i < 10; ++i) {
		pi = pointpool.alloc();
		*pi = point(i, i);
		ASSERT_EQ(*pi, point(i, i));
	}
}

TEST_F(PoolTesting, PointVarAlloc) {
	pool<point> pointpool(10);
	point temp;
	point* pi = &temp;
	for (size_t i = 0; i < 10; ++i) {
		pi = pointpool.alloc(i, i);
		ASSERT_EQ(*pi, point(i, i));
	}
	for (size_t i = 0; i < 10; ++i) {
		ASSERT_NO_THROW(pointpool.free(pi - i));
	}
}

TEST_F(PoolTesting, Int) {
	pool<int> intpool(10);
	int* pi[10];
	for (size_t i = 0; i < 10; ++i) {
		pi[i] = intpool.alloc();
		*(pi[i]) = i;
		ASSERT_EQ(*(pi[i]), i);
	}
	for (size_t i = 0; i < 10; ++i) {
		ASSERT_NO_THROW(intpool.free(pi[i]));
	}
	for (size_t i = 0; i < 10; ++i) {
		pi[i] = intpool.alloc();
		*(pi[i]) = i;
		ASSERT_EQ(*(pi[i]), i);
	}
}

TEST_F(PoolTesting, IntThrows) {
	pool<int> intpool(1);
	int * pi = intpool.alloc();
	ASSERT_ANY_THROW(pi = intpool.alloc());
	int out;
	ASSERT_ANY_THROW(intpool.free(&out));
}

TEST_F(PoolTesting, intRefTest) {
	pool<int&> intpool(10);
	int* pi[10];
	for (size_t i = 0; i < 10; ++i) {
		size_t& intref = i;
		pi[i] = intpool.alloc();
		*(pi[i]) = intref;
		ASSERT_EQ(*(pi[i]), i);
	}
	for (size_t i = 0; i < 10; ++i) {
		ASSERT_NO_THROW(intpool.free(pi[i]));
	}
	for (size_t i = 0; i < 10; ++i) {
		pi[i] = intpool.alloc();
		*(pi[i]) = i;
		ASSERT_EQ(*(pi[i]), i);
	}
}

TEST_F(PoolTesting, ArrayOfStringsTest) {
	pool<stupidString[3]> stringarrpool(5);
	stupidString strarr[3] = {"abc", "bcd", "cde"};
	auto res = stringarrpool.alloc(strarr);
	auto res2 = stringarrpool.alloc(strarr);
	stringarrpool.free(res);
	stringarrpool.free(res2);
	stringarrpool.alloc(strarr);
	stringarrpool.alloc(strarr);
	res = stringarrpool.alloc(strarr);
	stringarrpool.alloc(strarr);
	stringarrpool.free(res);
}

TEST_F(PoolTesting, ArrayOfPointersTest) {
	pool<stupidString*[3]> pool(5);
	stupidString strarr[3] = { "abc", "bcd", "cde" };
	stupidString* parr[3] = { &strarr[0], &strarr[1], &strarr[2] };	
	
	auto res1 = pool.alloc(parr);
	pool.free(res1);
}

TEST_F(PoolTesting, MultDimArrayTest) {
	pool<stupidString[2][3]> pool(5);
	stupidString strarr[2][3] = { { "abc", "bcd", "cde" }, { "abc", "bcd", "cde" } };
	stupidString* parr[2][3] = { {&strarr[0][0], &strarr[0][1], &strarr[0][2]}, {&strarr[1][0], &strarr[1][1], &strarr[1][2]} };

	auto res1 = pool.alloc(strarr);
	pool.free(res1);
	res1 = pool.alloc(strarr);
	res1 = pool.alloc(strarr);
	res1 = pool.alloc();
	res1 = pool.alloc(strarr);
	pool.alloc();
	pool.free(res1);
	pool.alloc(strarr);
}



TEST_F(PoolTesting, MinusOneAndZeroTest) {
	//pool<int> minuspool(-1);
	pool<int> zeropool(0);
	ASSERT_ANY_THROW(zeropool.alloc(5));
}

TEST_F(PoolTesting, PoolOfPools) {
	pool<pool<pool<int>>> pool_of_pools_of_pools_of_int(3);
	//pool_of_pools_of_pools_of_int.alloc();
}


int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}

#include "pool.h"

#include <iostream>
#include <gtest/gtest.h>

#include <crtdbg.h> 

#include "persons.h"

class point final {
public:
	int x, y;
	point() {
		x = 0; 
		y = 0;
	}
	point(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

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
	pool<char*> charastpool(10);
	for (size_t i = 0; i < 10; ++i) {
		charastpool.alloc("gsg");
	}
}

//TEST_F(PoolTesting, Point) {
//	pool<point> pointpool(10);
//	point temp;
//	point* pi = &temp;
//	for (size_t i = 0; i < 10; ++i) {
//		pi = pointpool.alloc();
//		*pi = point(i, i);
//		ASSERT_EQ(*pi, point(i, i));
//	}
//	for (size_t i = 0; i < 10; ++i) {
//		ASSERT_NO_THROW(pointpool.free(pi - i));
//	}
//	for (size_t i = 0; i < 10; ++i) {
//		pi = pointpool.alloc();
//		*pi = point(i, i);
//		ASSERT_EQ(*pi, point(i, i));
//	}
//}
//
//TEST_F(PoolTesting, PointVarAlloc) {
//	pool<point> pointpool(10);
//	point temp;
//	point* pi = &temp;
//	for (size_t i = 0; i < 10; ++i) {
//		pi = pointpool.alloc(i, i);
//		ASSERT_EQ(*pi, point(i, i));
//	}
//	for (size_t i = 0; i < 10; ++i) {
//		ASSERT_NO_THROW(pointpool.free(pi - i));
//	}
//}
//
//TEST_F(PoolTesting, Int) {
//	pool<int> intpool(10);
//	int temp;
//	int* pi = &temp;
//	for (size_t i = 0; i < 10; ++i) {
//		pi = intpool.alloc();
//		*pi = i;
//		ASSERT_EQ(*pi, i);
//	}
//	for (size_t i = 0; i < 10; ++i) {
//		ASSERT_NO_THROW(intpool.free(pi - i));
//	}
//	for (size_t i = 0; i < 10; ++i) {
//		pi = intpool.alloc();
//		*pi = i;
//		ASSERT_EQ(*pi, i);
//	}
//}
//
//TEST_F(PoolTesting, IntThrows) {
//	pool<int> intpool(1);
//	int * pi = intpool.alloc();
//	ASSERT_ANY_THROW(pi = intpool.alloc());
//	int out;
//	ASSERT_ANY_THROW(intpool.free(&out));
//}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);

	return RUN_ALL_TESTS();
}
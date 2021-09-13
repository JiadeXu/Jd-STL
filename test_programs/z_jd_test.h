/*************************************************************************
	> File Name: jd_test.h
	> Author: 
	> Mail: 
	> Created Time: Mon Sep 13 08:40:57 2021
 ************************************************************************/

#ifndef _JD_TEST_H
#define _JD_TEST_H

#include <cassert>
#include <mutex>
#include <iostream>
#include <string>
#include <vector>
#include <cassert>
#include <sstream>

class TestBase {
public:
	virtual ~TestBase() {}
	virtual void testBody() = 0;
};

class TestInfo;
class TestFactoryBase {
public:
	virtual TestBase* getTestObject() = 0;
	virtual void destory(TestBase *) = 0;
	virtual ~TestFactoryBase() {}
};
template<class CLASS_T>
class TestFactoryBaseImpl : public TestFactoryBase {
public:
	TestBase * getTestObject() override {
		return new CLASS_T();
	}
	void destory(TestBase *base) override {
		assert(dynamic_cast<CLASS_T *>(base) != nullptr);
		delete base;
	}
};

class TestUnit {
public:
	static TestUnit* getInstance();
	void addTestInfo(TestInfo *a) {
		test_info_arr.push_back(a);
	}
	void setCurTestInfo(TestInfo *info) {
		curTestInfo = info;
	}
	TestInfo* getCurTestInfo() {
		return curTestInfo;
	}
	std::vector<TestInfo *>::iterator begin() { return test_info_arr.begin(); }
	std::vector<TestInfo *>::iterator end() { return test_info_arr.end(); }
private:
	TestUnit() {}
	TestUnit(const TestUnit &) = delete;
	static TestUnit *single;
	TestInfo *curTestInfo;
	std::vector<TestInfo *> test_info_arr;
	static std::mutex m_mutex;
};
std::mutex TestUnit::m_mutex;
TestUnit* TestUnit::single = nullptr;
TestUnit* TestUnit::getInstance() {
	if (single == nullptr) {
		std::unique_lock<std::mutex> lock(m_mutex);
		if (single == nullptr) {
			single = new TestUnit();
		}
	}
	return single;
}

class TestInfo {
public:
	TestInfo(
		const char *test_suit_name,
		const char *test_name,
		TestFactoryBase *factory
	): test_suit_name(test_suit_name), test_name(test_name), factory(factory) {}
	void run() {
		TestBase *base = factory->getTestObject();
		cur_flag = true;
		TestUnit::getInstance()->setCurTestInfo(this);
		std::cout << "[ RUN ] " << test_suit_name << "." << test_name << std::endl;
		base->testBody();
		if (cur_flag) {
			std::cout << "[ OK ] ";
		} else {
			std::cout << "[ FAILED ] ";
		}
		std::cout << test_suit_name << "." << test_name << std::endl;
		factory->destory(base);
		TestUnit::getInstance()->setCurTestInfo(nullptr);
	}
	bool cur_flag;
private:
	const char *test_suit_name;
	const char *test_name;
	TestFactoryBase *factory;
};

TestInfo* MakeAndRegisterTestInfo(const char *test_suit_name, const char *test_name, TestFactoryBase *factory) {
	TestInfo *info = new TestInfo(test_suit_name, test_name, factory);
	TestUnit::getInstance()->addTestInfo(info);
	return info;
}
class CompMsg {
public:
	CompMsg(): _msg("") {}
	template<class U, class T>
	CompMsg(
		const T &a,
		const U &b,
		const char *str_a,
		const char *comp_str,
		const char *str_b,
		const char *file_name,
		int position
	) {
		std::stringstream ss;
		ss << file_name << ":" << position << " Failure\n";
		ss << "Expected (" << str_a << ") " << comp_str << " (" << str_b << "), actual: " << a << " vs " << b << "\n";
		_msg = ss.str();
	}
	const char* getMsg() { return _msg.c_str(); }
	operator bool() {
		return _msg != "";
	}
private:
	std::string _msg;
};

template<class T, class U>
CompMsg CompareEQ(const T &a, const U &b, const char *str_a, const char *str_b, const char *file_name, int position) {
	if (a == b) {
		return CompMsg();
	}
	return CompMsg(a, b, str_a, "==", str_b, file_name, position);
}

template<class T, class U>
CompMsg CompareNE(const T &a, const U &b, const char *str_a, const char *str_b, const char *file_name, int position) {
	if (a != b) {
		return CompMsg();
	}
	return CompMsg(a, b, str_a, "!=", str_b, file_name, position);
}

template<class T, class U>
CompMsg CompareLT(const T &a, const U &b, const char *str_a, const char *str_b, const char *file_name, int position) {
	if (a < b) {
		return CompMsg();
	}
	return CompMsg(a, b, str_a, "<", str_b, file_name, position);
}

template<class T, class U>
CompMsg CompareLE(const T &a, const U &b, const char *str_a, const char *str_b, const char *file_name, int position) {
	if (a <= b) {
		return CompMsg();
	}
	return CompMsg(a, b, str_a, "<=", str_b, file_name, position);
}

template<class T, class U>
CompMsg CompareGT(const T &a, const U &b, const char *str_a, const char *str_b, const char *file_name, int position) {
	if (a > b) {
		return CompMsg();
	}
	return CompMsg(a, b, str_a, ">", str_b, file_name, position);
}

template<class T, class U>
CompMsg CompareGE(const T &a, const U &b, const char *str_a, const char *str_b, const char *file_name, int position) {
	if (a >= b) {
		return CompMsg();
	}
	return CompMsg(a, b, str_a, ">=", str_b, file_name, position);
}

#define EXPECT(a, b, comp) {									\
	CompMsg compMsg = comp(a, b, #a, #b, __FILE__, __LINE__);	\
	if (compMsg) {												\
		std::cout << compMsg.getMsg() << std::endl;				\
		if (TestUnit::getInstance()->getCurTestInfo()) {				\
			TestUnit::getInstance()->getCurTestInfo()->cur_flag = false;	\
		}														\
	}																\
}

#define EXPECT_EQ(a, b) EXPECT(a, b, CompareEQ)
#define EXPECT_NE(a, b) EXPECT(a, b, CompareNE)
#define EXPECT_LT(a, b) EXPECT(a, b, CompareLT)
#define EXPECT_LE(a, b) EXPECT(a, b, CompareLE)
#define EXPECT_GT(a, b) EXPECT(a, b, CompareGT)
#define EXPECT_GE(a, b) EXPECT(a, b, CompareGE)

#define TEST_CLASS_NAME(a, b) JD_##a##_##b##_TEST_CLASS

#define TEST(test_suit_name, test_name)									\
class TEST_CLASS_NAME(test_suit_name, test_name) : public TestBase {	\
public:																	\
	void testBody() override;											\
private:																\
	static TestInfo* test_info;											\
};																		\
TestInfo* TEST_CLASS_NAME(test_suit_name, test_name)::test_info 		\
	= MakeAndRegisterTestInfo(											\
		#test_suit_name,													\
		#test_name, 														\
		new TestFactoryBaseImpl<TEST_CLASS_NAME(test_suit_name, test_name)>()	\
	);																	\
void TEST_CLASS_NAME(test_suit_name, test_name)::testBody()

void RUN_ALL_TESTS() {
	for (auto t : (*TestUnit::getInstance())) {
		t->run();
	}
}

#endif
